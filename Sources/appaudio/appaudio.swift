import SwiftUI
import CAudio

final class Livecycle: ObservableObject {
  init() { audio_setup() }
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
