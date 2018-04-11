/*
 *  Copyright (c) 2018 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "sdk/android/generated_java_audio_device_jni/jni/JavaAudioDeviceModule_jni.h"
#include "sdk/android/src/jni/audio_device/audio_record_jni.h"
#include "sdk/android/src/jni/audio_device/audio_track_jni.h"
#include "sdk/android/src/jni/jni_helpers.h"

namespace webrtc {
namespace jni {

static jlong JNI_JavaAudioDeviceModule_CreateAudioDeviceModule(
    JNIEnv* env,
    const JavaParamRef<jclass>& j_caller,
    const JavaParamRef<jobject>& j_context,
    const JavaParamRef<jobject>& j_audio_manager,
    const JavaParamRef<jobject>& j_webrtc_audio_record,
    const JavaParamRef<jobject>& j_webrtc_audio_track,
    int sample_rate,
    jboolean j_use_stereo_input,
    jboolean j_use_stereo_output) {
  AudioParameters input_parameters;
  AudioParameters output_parameters;
  android_adm::GetAudioParameters(env, j_context, j_audio_manager, sample_rate,
                                  j_use_stereo_input, j_use_stereo_output,
                                  &input_parameters, &output_parameters);
  auto audio_input = rtc::MakeUnique<android_adm::AudioRecordJni>(
      env, input_parameters,
      android_adm::kHighLatencyModeDelayEstimateInMilliseconds,
      j_webrtc_audio_record);
  auto audio_output = rtc::MakeUnique<android_adm::AudioTrackJni>(
      env, output_parameters, j_webrtc_audio_track);
  return jlongFromPointer(
      CreateAudioDeviceModuleFromInputAndOutput(
          AudioDeviceModule::kAndroidJavaAudio, j_use_stereo_input,
          j_use_stereo_output,
          android_adm::kHighLatencyModeDelayEstimateInMilliseconds,
          std::move(audio_input), std::move(audio_output))
          .release());
}

}  // namespace jni
}  // namespace webrtc