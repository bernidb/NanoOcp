import SwiftUI

struct Device: Identifiable {
    let id: Int
    let name: String
    let type: String
    let remId: String
    let IP: String
}
struct ListView : View {
    @State var deviceList = [
        Device(id: 0, name: "Main L", type: "D40", remId: "0.01", IP: "192.168.1.111"),
        Device(id: 1, name: "Main R", type: "D40", remId: "0.02", IP: "192.168.1.112"),
        Device(id: 2, name: "Monitor", type: "40D", remId: "0.03", IP: "192.168.1.113"),
        Device(id: 3, name: "FOH", type: "5D", remId: "0.04", IP: "192.168.1.114"),
    ]
      
    var body: some View {
        NavigationView {
            List(deviceList) { Device in
                HStack {
                    Text(Device.name)//.foregroundColor(Color.white)
                    Spacer(minLength: 10)
                    Text(Device.type)//.foregroundColor(Device.color)
                }//.listRowBackground(Color.black.opacity(0.75))
            }
            .navigationBarTitle(Text("Devices"))
            .navigationBarItems(
                trailing: Button(action: addDevice, label: { Text("Add") })
            )
        }
    }
    
    func addDevice() {
        if let randomDevice = deviceList.randomElement() {
            deviceList.append(randomDevice)
        }
    }
}
