package main

import (
	"log"
	"sync"
	"time"

	mqtt "github.com/eclipse/paho.mqtt.golang"
	nats_server "github.com/nats-io/nats-server/v2/server"
	"github.com/nats-io/nats.go"
)

func main() {
	log.Println("Hi World")

	ns, err := nats_server.NewServer(&nats_server.Options{
		Users: []*nats_server.User{
			{Username: "pint", Password: "pint"},
		},
		JetStream: true,
		MQTT: nats_server.MQTTOpts{
			Port: 1883,
		},
	})
	if err != nil {
		log.Fatalln(err)
	}
	go ns.Start()
	if !ns.ReadyForConnections(4 * time.Second) {
		log.Fatalln("NATS server startup timeout")
	}
	log.Printf("NATS server available at %s\n", ns.ClientURL())
	log.Println("MQTT server available at mqtt://0.0.0.0:1883")

	var wg sync.WaitGroup

	wg.Add(1)
	go func() {
		nc, err := nats.Connect(ns.ClientURL(),
			nats.UserInfo("pint", "pint"))
		if err != nil {
			log.Fatalln(err)
		}
		log.Println("NATS client connection success")
		nc.Close()
		wg.Done()
	}()

	wg.Add(1)
	go func() {
		mopt := mqtt.NewClientOptions()
		mopt.AddBroker(":1883")
		mopt.Username = "pint"
		mopt.Password = "pint"
		mc := mqtt.NewClient(mopt)
		t := mc.Connect()
		t.Wait()
		if err := t.Error(); err != nil {
			log.Fatalln(err)
		}
		log.Println("MQTT client connection success")
		mc.Disconnect(4 * 1000)
		wg.Done()
	}()

	wg.Wait()
	ns.Shutdown()
	ns.WaitForShutdown()
}
