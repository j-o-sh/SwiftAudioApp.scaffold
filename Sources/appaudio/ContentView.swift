import SwiftUI
import CAudio

struct ContentView: View {
    var body: some View {
        Text("Hello, World!")
            .padding(16)
        Button("Test") {
            print("❤️‍🔥 Hi!")
            let ok = audio_setup();
            print("Audio lib says ", ok)
        }
    }
}
