<script setup lang="ts">
import { computed, ref } from 'vue'
import EChart from './EChart.vue'

const legitIds = new Set([
  'admin',
  'user123',
  'secretCode',
  'testUser',
  '12345678',
])

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
</script>

<template>
  <div
    class="py-2 absolute w-full flex items-center justify-center"
    v-if="!isLegitDevId(lastInputDevId)"
  >
    <img
      style="height: 40vh; border-radius: 16px; border: 2px solid white"
      src="../assets/anton.png"
      alt="Lega"
    />
  </div>
  <div
    class="flex column items-center justify-end w-fit p-4 mx-auto transition-all"
    :class="{
      'min-h-1': isLegitDevId(lastInputDevId),
      'min-h-screen': !isLegitDevId(lastInputDevId),
      'min-w-1': !isLegitDevId(lastInputDevId),
      'min-w-screen': isLegitDevId(lastInputDevId),
    }"
  >
    <input
      maxlength="8"
      placeholder="Enter your glove ID"
      autofocus
      class="border-[#2a3c70]/80 text-center outline-1 outline-slate-300-[#2a3c70] rounded-full shadow-xl shadow-blue-500/35 transition-all"
      :class="{
        'px-16': !isLegitDevId(lastInputDevId),
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
    v-if="isLegitDevId(lastInputDevId)"
    class="grid grid-flow-col grid-cols-2 gap-2 p-2 container mx-auto"
  >
    <EChart
      class="w-full aspect-[2/1] bg-blend-color rounded-2xl shadow-xl shadow-blue-500/50 p-6 border border-[#2a3c70]/80"
      :url="`/api/devices/${lastLegitDevId}/input/live/`"
      :maxPoints="20"
      :options="{
        animation: false,
        darkMode: true,
        color: '#00bcff',

        textStyle: {
          color: '#fff',
        },
        title: {
          text: 'Input tremor signal',
          left: 'center',
          textStyle: {
            color: '#fff',
          },
        },
        xAxis: {
          type: 'time',
          scale: true,
          boundaryGap: false,
          areaStyle: {},
        },
        yAxis: {
          type: 'value',
          scale: true,
          splitLine: {
            lineStyle: {
              color: '#364459',
            },
          },
        },
        series: [
          {
            type: 'line',
            symbol: 'none',
            areaStyle: { color: 'rgba(0, 188, 255, 0.35)' },
            smooth: true,
          },
        ],
      }"
    />
    <EChart
      class="w-full aspect-[2/1] bg-blend-color rounded-2xl shadow-xl shadow-blue-500/50 p-6 border border-[#2a3c70]/80"
      :url="`/api/devices/${lastLegitDevId}/inverted/live/`"
      :maxPoints="20"
      :options="{
        animation: false,
        darkMode: true,
        color: '#00bcff',
        textStyle: {
          color: '#fff',
        },
        title: {
          text: 'Countered tremor signal',
          left: 'center',
          textStyle: {
            color: '#fff',
          },
        },
        xAxis: { type: 'time', scale: true },
        yAxis: {
          type: 'value',
          scale: true,
          splitLine: {
            lineStyle: {
              color: '#364459',
            },
          },
        },
        series: [
          {
            areaStyle: { color: 'rgba(0, 188, 255, 0.35)' },
            symbol: 'none',
            type: 'line',
            smooth: true,
          },
        ],
      }"
    />
    <EChart
      class="w-full row-span-2 aspect-[1/1] bg-blend-color rounded-2xl shadow-xl shadow-blue-500/50 p-6 border border-[#2a3c70]/80"
      :url="`/api/devices/${lastLegitDevId}/input/hourly/`"
      :maxPoints="1"
      :options="{
        darkMode: true,
        color: '#00bcff',
        textStyle: {
          color: '#fff',
        },
        title: {
          text: 'Average hourly amplitude',
          left: 'center',
          textStyle: {
            color: '#fff',
          },
        },
        radar: {
          indicator: [
            { name: '00' },
            { name: '01' },
            { name: '02' },
            { name: '03' },
            { name: '04' },
            { name: '05' },
            { name: '06' },
            { name: '07' },
            { name: '08' },
            { name: '09' },
            { name: '10' },
            { name: '11' },
            { name: '12' },
            { name: '13' },
            { name: '14' },
            { name: '15' },
            { name: '16' },
            { name: '17' },
            { name: '18' },
            { name: '19' },
            { name: '20' },
            { name: '21' },
            { name: '22' },
            { name: '23' },
          ],
        },
        series: [
          {
            type: 'radar',
            areaStyle: { color: 'rgba(0, 188, 255, 0.35)' },
          },
        ],
      }"
    />
  </div>
</template>
