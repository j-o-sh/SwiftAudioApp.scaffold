import SwiftUI
import CAudio

struct ContentView: View {
  var body: some View {
    VStack {
      Text("Hello, World!").padding(16)
      Button("setup") { audio_setup() }
      Button("create") { create_recording(5) }
      Button("Rec") { record() }
      Button("Play") { play() }
    }
  }
}
