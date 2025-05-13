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

const timeline = computed(() => {
  const search = searchEl.value
  const searchWrapper = searchWrapperEl.value
  const charts = chartsEl.value
  if (!search || !searchWrapper || !charts) return
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
      charts,
      { autoAlpha: 0, display: 'none' },
      { autoAlpha: 1, display: 'grid' },
      '<',
    )
})

watchEffect(() => {
  const tl = timeline.value
  if (!tl) return
  if (isLegitDevId(lastInputDevId.value)) {
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
    ref="chartsEl"
  >
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full col-span-2" />
    <img :src="chartImg" class="w-full" />
    <img :src="chartImg" class="w-full" />
  </div>
</template>
