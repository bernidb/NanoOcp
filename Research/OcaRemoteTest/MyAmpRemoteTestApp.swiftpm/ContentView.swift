import SwiftUI
import Network

struct ContentView: View {
    
    @State private var isEditing = false
    @State private var currentValue = 0.0
    
    var body: some View {
        
        VStack {

            HStack {
                Image(systemName: "globe")
                    .imageScale(.large)
                    .foregroundColor(.accentColor)
                
                Text("Hello, world!")
                    .foregroundColor(.white)
                
                Button(action: {doIt()},
                       label: {
                        Text("Do it!")
                            .fontWeight(.heavy)
                            .foregroundColor(.black)
                            .padding()})
                    //.cornerRadius(15)
                    .background(Color.green)
            }
            .background(Color.gray)
            
            VStack {
                Slider(
                        value: $currentValue,
                        in: -120...24,
                        step: 1
                    ) {
                        Text("Speed")
                    } minimumValueLabel: {
                        Text("-120 dB")
                            .foregroundColor(.white)
                    } maximumValueLabel: {
                        Text("24 dB")
                            .foregroundColor(.white)
                    } onEditingChanged: { editing in
                        isEditing = editing
                    }
                Text("\(currentValue) dB")
                    .foregroundColor(isEditing ? .red : .white).padding(.bottom, 50)
                
                //Spacer(minLength: 10)
                
                Text("final text")
                    .foregroundColor(.white)
            }
            .background(Color.black)
        }
    }
    
    func setValue() {
        
    }
    
    func doIt() {
        //nw_parameters_configure_protocol_block_t configure_tls = //NW_PARAMETERS_DISABLE_PROTOCOL;
//
        //parameters = nw_parameters_create_secure_tcp(
        //    configure_tls,
        //    NW_PARAMETERS_DEFAULT_CONFIGURATION);
//
        //nw_endpoint_t endpoint = nw_endpoint_create_host(name, //port);
        //nw_connection_t connection = nw_connection_create(endpoint, //parameters);
        //count += 1
    }
}
