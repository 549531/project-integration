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

function onResize() {
  chart?.resize()
}

onMounted(() => {
  chart = echarts.init(containerRef.value)
  chart?.setOption(options)

  window.addEventListener('resize', onResize)
})

watchEffect(() => {
  sse?.close()
  sse = new EventSource(url)
  sse.onmessage = (event) => {
    const numbers = JSON.parse(event.data)
    for (const point of numbers.data) {
      data.push(point)
    }
    while (data.length > maxPoints) data.shift()
    chart?.setOption(
      { series: { data: data } },
      {  lazyUpdate: true },
    )
  }
})

onUnmounted(() => {
  sse?.close()
  window.removeEventListener('resize', onResize)
})
</script>

<template>
  <div ref="container"></div>
</template>
