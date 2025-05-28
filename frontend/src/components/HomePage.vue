<script setup lang="ts">
import {computed, ref, watchEffect} from 'vue'
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
const echartRef = ref<HTMLDivElement>()

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
        defaults: {duration: 0.25, ease: 'power4'},
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
          {autoAlpha: 0, display: 'none'},
          {autoAlpha: 1, display: 'grid'},
          '<',
      )
})

watchEffect(async () => {
  const tl = timeline.value
  console.log('watchEffect triggered — deviceId:', deviceId.value)
  console.log('searchSmall:', searchSmall.value)
  if (!tl) return
  if (searchSmall.value) {
    await tl.play()

    if (echartRef.value) {
      console.log(' Chart block triggered. echartRef:', echartRef.value)

      try {
        const res = await fetch('/random/api/v1.0/random?min=100&max=1000&count=7')
        if (!res.ok) {
          throw new Error(`Fetch failed: ${res.status} ${res.statusText}`)
        }

        const numbers = await res.json()
        console.log('Fetched numbers:', numbers)

        const chartInstance = echarts.init(echartRef.value)
        chartInstance.setOption({
          xAxis: {
            type: 'category',
            data: ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'],
          },
          yAxis: {
            type: 'value',
          },
          series: [
            {
              data: numbers,
              type: 'line',
              smooth: true,
            },
          ],
        })
      } catch (error) {
        console.error('Error fetching chart data:', error)
      }
    }
  } else {
    await tl.reverse()
    if (echartRef.value) {
      echarts.dispose(echartRef.value)
    }
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
    <div class="w-full col-span-2 aspect-[2/1]" ref="echartRef" ></div>
    <img :src="chartImg" class="w-full"/>
    <img :src="chartImg" class="w-full"/>
    <img :src="chartImg" class="w-full col-span-2"/>
    <img :src="chartImg" class="w-full"/>
    <img :src="chartImg" class="w-full"/>
  </div>
</template>


