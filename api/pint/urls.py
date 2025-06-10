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
    path(
        "devices/<int:device_id>/<str:property>/csv/",
        views.csv_export,
        name="csv_export",
    ),
    path(
        "devices/<int:device_id>/<str:property>/hourly/",
        views.hourly_statistics,
        name="hourly_statistics",
    ),
]
