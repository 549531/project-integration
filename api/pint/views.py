from datetime import datetime, timedelta, timezone
from django.http import HttpResponse, JsonResponse, StreamingHttpResponse
from random import randint
import json
import asyncio
import nats

# Create your views here.


def index(request):
    return HttpResponse("hi world")


def property(request, device_id, property):
    values = [randint(0, 100) for _ in range(500)]
    return JsonResponse({"values": values})


async def property_live(request, device_id, property):
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
        sub = await js.pull_subscribe(f"devices.{device_id}.{property}", config=conf)

        try:
            while msgs := await sub.fetch(batch=1024):
                res = {
                    "data": [
                        {
                            "name": int(msg.metadata.timestamp.timestamp() * 1000),
                            "value": [
                                int(msg.metadata.timestamp.timestamp() * 1000),
                                float(msg.data),
                            ],
                        }
                        for msg in msgs
                    ]
                }
                yield "data: "
                yield json.dumps(res)
                yield "\n\n"
        except TimeoutError:
            yield "\n\n"
        except asyncio.CancelledError:
            pass

        await sub.unsubscribe()
        await nc.close()

    return StreamingHttpResponse(
        response(),
        content_type="text/event-stream",
        headers={"Cache-Control": "no-cache"},
    )


async def csv_export(request, device_id, property):
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
            opt_start_time=(datetime.now(timezone.utc) - timedelta(days=7)).isoformat(),
        )
        sub = await js.pull_subscribe(f"devices.{device_id}.{property}", config=conf)

        yield "time,value\n"
        try:
            while msgs := await sub.fetch(batch=1024, timeout=0.1):
                for msg in msgs:
                    time = msg.metadata.timestamp.strftime("%F %T")
                    value = float(msg.data)
                    yield f"{time},{value}\n"
        except TimeoutError:
            pass
        except asyncio.CancelledError:
            pass

        await sub.unsubscribe()
        await nc.close()

    return StreamingHttpResponse(
        response(),
        content_type="text/csv",
        headers={"Cache-Control": "no-cache"},
    )


async def hourly_statistics(request, device_id, property):
    async def response():
        nc = await nats.connect(
            servers="tls://jarkad.net.eu.org",
            tls_handshake_first=True,
            user="pint",
            password="pint",
        )
        js = nc.jetstream()

        size = 60
        sum = [0] * size
        count = [0] * size
        res = [0] * size

        opts = {}
        try:
            last = json.loads(request.headers["last-event-id"])
            res = last["res"]
            opts["deliver_policy"] = nats.js.api.DeliverPolicy.BY_START_SEQUENCE
            opts["opt_start_seq"] = last["seq"]
        except:
            opts["deliver_policy"] = nats.js.api.DeliverPolicy.BY_START_TIME
            opts["opt_start_time"] = (
                datetime.now(timezone.utc) - timedelta(days=30)
            ).isoformat()

        conf = nats.js.api.ConsumerConfig(
            mem_storage=True,
            ack_policy=nats.js.api.AckPolicy.NONE,
            **opts,
        )
        sub = await js.pull_subscribe(f"devices.{device_id}.{property}", config=conf)

        try:
            while msgs := await sub.fetch(batch=1024):
                for msg in msgs:
                    time = msg.metadata.timestamp.second
                    value = float(msg.data)
                    sum[time] += value
                    count[time] += 1
                    res[time] = sum[time] / count[time]
                if msgs:
                    seq = msgs[-1].metadata.sequence.stream
                    yield "id: "
                    yield json.dumps({"seq": seq, "res": res})
                    yield "\n"
                yield "data: "
                yield json.dumps({"data": [res]})
                yield "\n\n"
        except TimeoutError:
            yield "\n\n"
        except asyncio.CancelledError:
            pass

        await sub.unsubscribe()
        await nc.close()

    return StreamingHttpResponse(
        response(),
        content_type="text/event-stream",
        headers={"Cache-Control": "no-cache"},
    )
