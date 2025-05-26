from django.urls import path
from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("devices/<int:device_id>/amplitude/", views.amplitude, name="amplitude"),
    path(
        "devices/<int:device_id>/amplitude/live/",
        views.amplitude_live,
        name="amplitude_live",
    ),
]
