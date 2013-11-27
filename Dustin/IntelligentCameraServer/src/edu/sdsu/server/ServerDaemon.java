package edu.sdsu.server;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import android.hardware.Camera;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;
import edu.sdsu.cameraserver.CameraServer;
import edu.sdsu.server.util.EncoderActivationInterface;
/**
 * This is a TCP server which will listen for connections on port 7744 and spawn worker threads
 * upon each accepted connection.
 *
 */
public class ServerDaemon extends AsyncTask<Void, Void, Void>{
	//private List<ServerThread>	connections = new ArrayList<ServerThread>();
	//The current encoder
	private EncoderActivationInterface			m_encoder;
	
	public ServerDaemon(EncoderActivationInterface encoder)
	{ 
		//Test
		//Use same interface as in UI
		m_encoder = encoder;
	}

	@Override
	protected Void doInBackground(Void... params) {
		try {
			ServerSocket socket = new ServerSocket();
			socket.setReuseAddress(true);
			//socket.setSoTimeout(10000);
			socket.bind(new InetSocketAddress(7744));
			
			while(!socket.isClosed())
			{
				System.out.println("Before accept....");
				Socket skt = socket.accept();
				//Start streaming
				System.out.println("Connected! Host IP: " + skt.getInetAddress());
				//Use same encoder interface as in UI
				ServerThread connection = new ServerThread(skt, m_encoder);
				connection.start();
				//connections.add(connection);
			}
			socket.close();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}

	

}