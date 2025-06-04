from django.urls import path
from . import views

urlpatterns = [
    path("", views.index, name="index"),
    path("devices/<int:device_id>/<str:property>/", views.property, name="property"),
    path(
        "devices/<int:device_id>/<str:property>/live/",
        views.property_live,
        name="property_live",
    ),
]
