from django.http import HttpResponse, JsonResponse
from random import randint

# Create your views here.


def index(request):
    return HttpResponse("hi world")


def amplitude(request, device_id):
    values = [randint(0, 100) for _ in range(500)]
    return JsonResponse({"values": values})
