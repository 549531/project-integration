from datetime import datetime, timedelta, timezone
from django.http import HttpResponse, JsonResponse, StreamingHttpResponse
from random import randint
import json
import nats

# Create your views here.


def index(request):
    return HttpResponse("hi world")


def amplitude(request, device_id):
    values = [randint(0, 100) for _ in range(500)]
    return JsonResponse({"values": values})


async def amplitude_live(request, device_id):
    async def response():
        nc = await nats.connect(
            servers="tls://jarkad.net.eu.org",
            tls_handshake_first=True,
            user="pint",
            password="pint",
        )
        js = nc.jetstream()

        conf = nats.js.api.ConsumerConfig(
            mem_storage=True,
            ack_policy=nats.js.api.AckPolicy.NONE,
            deliver_policy=nats.js.api.DeliverPolicy.BY_START_TIME,
            opt_start_time=(
                datetime.now(timezone.utc) - timedelta(minutes=2)
            ).isoformat(),
        )
        sub = await js.pull_subscribe(f"devices.{device_id}.amplitude", config=conf)

        try:
            while msgs := await sub.fetch(batch=1024):
                res = {
                    "data": [
                        {
                            "time": int(msg.metadata.timestamp.timestamp() * 1000),
                            "value": int(msg.data),
                        }
                        for msg in msgs
                    ]
                }
                yield "data: "
                yield json.dumps(res)
                yield "\n\n"
        except TimeoutError:
            yield "\n\n"

        await nc.close()

    return StreamingHttpResponse(
        response(),
        content_type="text/event-stream",
        headers={"Cache-Control": "no-cache"},
    )
