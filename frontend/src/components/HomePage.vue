<script setup lang="ts">
import { computed, onBeforeUnmount, type Ref, ref, watchEffect } from 'vue'
import * as echarts from 'echarts'
import chartImg from '../assets/chart.gif'

const legitIds = new Set([
  'admin',
  'user123',
  'secretCode',
  'testUser',
  '12345678',
])

const chartsEl = ref<HTMLInputElement>()
const echartRef = ref<HTMLDivElement | null>(null)
const echartRef_1 = ref<HTMLDivElement | null>(null)
let chartGraph: ReturnType<typeof graph> | null = null
let chartGraph_1: ReturnType<typeof graph> | null = null

const isLegitDevId = (devId: string | undefined) => {
  if (devId === undefined) return false
  const id = devId.trim()
  return legitIds.has(id)
}

const devIdModel = ref('')
const lastInputDevId = ref<string>()
const lastLegitDevId = computed<string | undefined>((previous) =>
  isLegitDevId(lastInputDevId.value) ? lastInputDevId.value : previous,
)

const lineChartOptions = {
  xAxis: { type: 'category' },
  yAxis: { type: 'value', scale: true },
  series: [{ type: 'line', smooth: true }],
}

function graph({
  echartRef,
  url = '/api/devices/123/amplitude/live/',
  chartOptions,
  maxPoints = 50,
}: {
  echartRef: Ref<HTMLDivElement | null>
  url?: string
  chartOptions: any
  maxPoints?: number
}) {
  let source: EventSource | null = null
  let chartInstance: echarts.ECharts | null = null

  const x_data: string[] = []
  const y_data: number[] = []

  function start() {
    if (!echartRef.value) return
    chartInstance = echarts.init(echartRef.value)
    chartInstance.setOption(chartOptions)
    source = new EventSource(url)
    source.onerror = (err) => {
      console.error('EventSource failed', err)
    }
    source.onmessage = (event) => {
      const numbers = JSON.parse(event.data)
      for (const point of numbers.data) {
        const timeLabel = new Date(point.time).toLocaleTimeString()
        x_data.push(timeLabel)
        y_data.push(point.value)
      }
      while (x_data.length > maxPoints) x_data.shift()
      while (y_data.length > maxPoints) y_data.shift()

      chartInstance?.setOption({
        xAxis: { data: x_data },
        series: [{ data: y_data }],
      })
    }
  }

  function stop() {
    if (chartInstance) chartInstance.dispose()
    if (source) source.close()
  }

  onBeforeUnmount(stop)

  return { start, stop }
}

watchEffect((onCleanup) => {
  if (isLegitDevId(lastInputDevId.value)) {
    requestAnimationFrame(() => {
      chartGraph = graph({
        echartRef,
        url: `/api/devices/${lastLegitDevId.value}/amplitude/live/`,
        chartOptions: lineChartOptions,
        maxPoints: 50,
      })
      chartGraph.start()
      chartGraph_1 = graph({
        echartRef: echartRef_1,
        url: `/api/devices/${lastLegitDevId.value}/frequency/live/`,
        chartOptions: lineChartOptions,
        maxPoints: 50,
      })
      chartGraph_1.start()
      onCleanup(() => {
        chartGraph?.stop()
        chartGraph = null
        chartGraph_1?.stop()
        chartGraph_1 = null
      })
    })
  } else {
    chartGraph?.stop()
    chartGraph = null
    chartGraph_1?.stop()
    chartGraph_1 = null
  }
})
</script>

<template>
  <div
    class="flex items-center justify-end w-fit p-4 mx-auto transition-all"
    :class="{
      'min-h-1': isLegitDevId(lastInputDevId),
      'min-h-screen': !isLegitDevId(lastInputDevId),
      'min-w-1': !isLegitDevId(lastInputDevId),
      'min-w-screen': isLegitDevId(lastInputDevId),
    }"
  >
    <input
      size="8"
      maxlength="8"
      placeholder="glove ID"
      autofocus
      class="border border-black-300 rounded-full shadow transition-all"
      :class="{
        'px-32': !isLegitDevId(lastInputDevId),
        'py-4': !isLegitDevId(lastInputDevId),
        'px-4': isLegitDevId(lastInputDevId),
      }"
      v-model="devIdModel"
      @keydown.enter="
        isLegitDevId(devIdModel) && ($event.target as HTMLInputElement).blur()
      "
      @focus="lastInputDevId = undefined"
      @blur="lastInputDevId = devIdModel"
    />
  </div>
  <div
    class="grid grid-flow-row auto-rows-auto grid-cols-2 gap-2 p-2 container mx-auto"
    :class="{ hidden: !isLegitDevId(lastInputDevId) }"
    ref="chartsEl"
  >
    <div class="w-full col-span-2 aspect-[2/1]" ref="echartRef"></div>
    <div class="w-full col-span-2 aspect-[2/1]" ref="echartRef_1"></div>
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full col-span-2" />
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full" />
  </div>
</template>
