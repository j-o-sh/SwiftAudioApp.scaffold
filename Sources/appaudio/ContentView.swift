import SwiftUI
import CAudio

struct ContentView: View {

  init() {
    create_recording(15)
  }

  var body: some View {
    VStack {
      Text("Hello, World!").padding(16)
      Button("Rec") { record() }
      Button("Play") { play() }
    }
  }
}
