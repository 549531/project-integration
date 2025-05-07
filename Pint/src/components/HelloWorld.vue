<script setup lang="ts">
import {ref, watchEffect} from 'vue'

defineProps<{ msg: string }>()

const legitIds = ['admin', 'user123', 'secretCode', 'testUser']

const device_id = ref('')
const isFocused = ref(false)
const loginStatus = ref('')

function checkLogin() {
  if (legitIds.includes(device_id.value.trim())) {
    loginStatus.value = 'success'
    alert('hi, ' + device_id.value)
  } else {
    loginStatus.value = 'fail'
    alert('NO, ' + device_id.value)
  }
}

watchEffect(() => {
  console.log(device_id.value);
})


</script>

<template>
  <div class="flex items-center justify-center min-h-screen ">
    <input
        class="border border-black-300 rounded-full px-4 shadow block m-auto"

        :placeholder="isFocused ? ' ':'login with device id'"
        v-model="device_id"
        @focus="isFocused = true"
        @blur="isFocused = false"
        @keydown.enter="checkLogin"
        style="height: 50px; width: 450px;"
    />
  </div>
</template>
