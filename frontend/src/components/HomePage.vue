<script setup lang="ts">
import { computed, onBeforeUnmount, type Ref, ref, watchEffect } from 'vue'
import gsap from 'gsap'
import * as echarts from 'echarts'
import chartImg from '../assets/chart.gif'

const legitIds = new Set([
  'admin',
  'user123',
  'secretCode',
  'testUser',
  '12345678',
])

const searchEl = ref<HTMLInputElement>()
const searchWrapperEl = ref<HTMLInputElement>()
const chartsEl = ref<HTMLInputElement>()
const echartRef = ref<HTMLDivElement | null>(null)
const echartRef_1 = ref<HTMLDivElement | null>(null)
let chartGraph: ReturnType<typeof graph> | null = null
let chartGraph_1: ReturnType<typeof graph> | null = null

const deviceIdRaw = ref('')
const deviceIdRaw_IsLegit = computed(() => {
  const id = deviceIdRaw.value.trim()
  return legitIds.has(id)
})
const deviceId = ref<string>()
const searchSmall = computed(() => deviceId.value !== undefined)

function checkLogin() {
  if (deviceIdRaw_IsLegit.value) {
    deviceId.value = deviceIdRaw.value.trim()
  } else {
    deviceId.value = undefined
  }
}

const timeline = computed(() => {
  const search = searchEl.value
  const searchWrapper = searchWrapperEl.value
  const chart = chartsEl.value
  if (!search || !searchWrapper || !chart) return
  return gsap
    .timeline({
      paused: true,
      defaults: { duration: 0.25, ease: 'power4' },
    })
    .fromTo(
      searchWrapper,
      {
        minWidth: 0,
        minHeight: '100vh',
      },
      {
        minWidth: '100vw',
        minHeight: 0,
      },
    )
    .fromTo(
      search,
      {
        paddingBlock: '1em',
        paddingInline: '8em',
      },
      {
        paddingBlock: 0,
        paddingInline: '1em',
      },
      '<',
    )
    .fromTo(
      chart,
      { autoAlpha: 0, display: 'none' },
      { autoAlpha: 1, display: 'grid' },
      '<',
    )
})

function lineChartOptions(x: string[], y: number[]): any {
  return {
    xAxis: { type: 'category', data: x },
    yAxis: { type: 'value' },
    series: [
      {
        data: y,
        type: 'line',
        smooth: true,
      },
    ],
  }
}

function graph({
  echartRef,
  url = '/random',
  chartOptionsFn: lineChartOptions,
  maxPoints = 50,
}: {
  echartRef: Ref<HTMLDivElement | null>
  url?: string
  chartOptionsFn: (x: string[], y: number[]) => any
  maxPoints?: number
}) {
  ///TODO: stay alive(optional)

  let source: EventSource | null = null
  let chartInstance: echarts.ECharts | null = null

  const x_data = ref<string[]>([])
  const y_data = ref<number[]>([])

  function start() {
    if (!echartRef.value) return
    chartInstance = echarts.init(echartRef.value)
    source = new EventSource(url)
    source.onerror = (err) => {
      console.error('EventSource failed', err)
    }
    source.onmessage = (event) => {
      const numbers = JSON.parse(event.data)
      for (const point of numbers.data) {
        const timeLabel = new Date(point.time).toLocaleTimeString()
        x_data.value.push(timeLabel)
        y_data.value.push(point.value)
      }
      while (x_data.value.length > maxPoints) x_data.value.shift()
      while (y_data.value.length > maxPoints) y_data.value.shift()

      if (chartInstance) {
        const chartOption = lineChartOptions(x_data.value, y_data.value)
        chartInstance.setOption(chartOption)
      }
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
  const tl = timeline.value
  console.log('watchEffect triggered — deviceId:', deviceId.value)
  console.log('searchSmall:', searchSmall.value)
  if (!tl) return
  if (searchSmall.value) {
    ;(async () => {
      await tl.play()
      chartGraph = graph({
        echartRef,
        url: '/random',
        chartOptionsFn: lineChartOptions,
        maxPoints: 50,
      })
      chartGraph.start()
      chartGraph_1 = graph({
        echartRef: echartRef_1,
        url: '/random',
        chartOptionsFn: lineChartOptions,
        maxPoints: 50,
      })
      chartGraph_1.start()
    })()
    onCleanup(() => {
      chartGraph?.stop()
      chartGraph = null
      chartGraph_1?.stop()
      chartGraph_1 = null
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
    class="flex items-center justify-end w-fit p-4 mx-auto"
    ref="searchWrapperEl"
  >
    <input
      size="8"
      maxlength="8"
      placeholder="glove ID"
      autofocus
      class="border border-black-300 rounded-full shadow"
      ref="searchEl"
      v-model="deviceIdRaw"
      @keydown.enter="
        deviceIdRaw_IsLegit && ($event.target as HTMLInputElement).blur()
      "
      @focus="deviceId = undefined"
      @blur="checkLogin()"
    />
  </div>
  <div
    class="grid grid-flow-row auto-rows-auto grid-cols-2 gap-2 p-2 container mx-auto"
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
