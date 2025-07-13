import SwiftUI
import AVFoundation
import CAudio

final class Livecycle: ObservableObject {
init() { 
    #if os(iOS)
    let session = AVAudioSession.sharedInstance()
    do {
      try session.setCategory(.playAndRecord, options: [.defaultToSpeaker])
      try session.setMode(.default)
      try session.setPreferredSampleRate(48000)
      try session.setPreferredIOBufferDuration(0.01)
      try session.setActive(true)
      print("✅ Audio session activated")

      session.requestRecordPermission { granted in 
        if granted {
          print("🎤 Microphone permission granted.")
        } else {
          print("🚫 Microphone permission denied.")
        }
      }

      print("Sesch: ", session.availableInputs, session.preferredInput)
    } catch {
      print("❌ Failed to activate audio session: \(error)")
    }
    #endif
  }
  deinit { audio_teardown() }
}

@main
struct appaudio: App {
  @StateObject private var livecycle = Livecycle()

  var body: some Scene {
    WindowGroup {
      ContentView()
        .frame(maxWidth: .infinity, maxHeight: .infinity)
    }
  }
}
