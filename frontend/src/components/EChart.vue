<script setup lang="ts">
import * as echarts from 'echarts'
import { watchEffect, useTemplateRef, onMounted, onUnmounted } from 'vue'

const { maxPoints, options, url } = defineProps({
  maxPoints: { type: Number, default: 100 },
  options: { type: Object, required: true },
  url: { type: String, required: true },
})

const containerRef = useTemplateRef('container')

let chart: echarts.ECharts | undefined
let sse: EventSource | undefined
let data: { name: string; value: [string, number] }[] = []

onMounted(() => {
  chart = echarts.init(containerRef.value)
  chart?.setOption(options)
})

watchEffect(() => {
  sse?.close()
  sse = new EventSource(url)
  sse.onmessage = (event) => {
    const numbers = JSON.parse(event.data)
    for (const point of numbers.data) {
      data.push({ name: point.time, value: [point.time, point.value] })
    }
    while (data.length > maxPoints) data.shift()
    chart?.setOption({ series: { data: data } })
  }
})

onUnmounted(() => {
  sse?.close()
})
</script>

<template>
  <div class="w-full col-span-2 aspect-[2/1]" ref="container"></div>
</template>
