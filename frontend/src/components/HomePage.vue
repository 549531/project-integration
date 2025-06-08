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
  <div class="py-2 absolute w-full flex items-center justify-center" v-if="!isLegitDevId(lastInputDevId)">
    <img style="height: 40vh; border-radius: 16px; border: 2px solid navy" src="../assets/anton.png" alt="Gandon">
  </div>
  <div class="flex column items-center justify-end w-fit p-4 mx-auto transition-all" :class="{
    'min-h-1': isLegitDevId(lastInputDevId),
    'min-h-screen': !isLegitDevId(lastInputDevId),
    'min-w-1': !isLegitDevId(lastInputDevId),
    'min-w-screen': isLegitDevId(lastInputDevId),
  }">
    <input maxlength="8" placeholder="Enter your glove ID" autofocus
      class="border text-center border-black-300 rounded-full shadow transition-all" :class="{
        'px-16': !isLegitDevId(lastInputDevId),
        'py-4': !isLegitDevId(lastInputDevId),
        'px-4': isLegitDevId(lastInputDevId),
      }" v-model="devIdModel" @keydown.enter="
        isLegitDevId(devIdModel) && ($event.target as HTMLInputElement).blur()
        " @focus="lastInputDevId = undefined" @blur="lastInputDevId = devIdModel" />
  </div>
  <div v-if="isLegitDevId(lastInputDevId)"
    class="grid grid-flow-row auto-rows-auto grid-cols-2 gap-2 p-2 container mx-auto">
    <EChart class="w-full col-span-1 aspect-[2/1]" :url="`/api/devices/${lastLegitDevId}/amplitude/live/`"
      :maxPoints="50" :options="{
        xAxis: { type: 'time', scale: true },
        yAxis: { type: 'value', scale: true },
        series: { type: 'line', smooth: true },
      }" />
    <EChart class="w-full col-span-1 aspect-[2/1]" :url="`/api/devices/${lastLegitDevId}/frequency/live/`"
      :maxPoints="50" :options="{
        xAxis: { type: 'time', scale: true },
        yAxis: { type: 'value', scale: true },
        series: { type: 'line', smooth: true },
      }" />

  </div>
</template>
