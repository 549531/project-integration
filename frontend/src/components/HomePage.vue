<script setup lang="ts">
import { computed, ref, watchEffect } from 'vue'
import gsap from 'gsap'

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

watchEffect(() => {
  const tl = timeline.value
  if (!tl) return
  if (searchSmall.value) {
    tl.play()
  } else {
    tl.reverse()
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
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full col-span-2" />
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full" />
  </div>
</template>
