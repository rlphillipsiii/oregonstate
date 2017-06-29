import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.ArrayList;

import javax.swing.*;

public class PhotoGUI extends JFrame{
	private JScrollPane pane = new JScrollPane();
	private ImagePanel panel = new ImagePanel();
	private JPanel backGround = new JPanel();
	private JPanel backGround2 = new JPanel();
	private JPanel thumbPan = new JPanel();
	private JButton imageButton;
	private static String fileName = "";
	private ArrayList<JButton> buttons = new ArrayList<JButton>();
	
	public PhotoGUI(){
		//sets the size and title properties for the PhotoGUI screen
		this.setTitle("Photo Browser");
		this.setLayout(null);
		this.setLocation(300, 100); 
		this.setSize(630, 525);
		this.setResizable(false);
		this.setBackground(Color.BLACK);
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		
		//creates a JScrollPane so that the you can scroll through the thumbnails
		pane.setSize(625, 100);
		pane.setLocation(0, 400);
		pane.setVisible(true);
		this.add(pane);
		pane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
		
		//creates a panel for the thumbnails to be stored in.
		FlowLayout flow = new FlowLayout(1);
		thumbPan.setBackground(Color.BLACK);
		thumbPan.setVisible(true);
		thumbPan.setLayout(flow);
		pane.setViewportView(thumbPan);

		//adds the images to the thumbPan if the variable fileName is something besides ""
		if(!fileName.equalsIgnoreCase("")){
			addImages();
		}
		
		panel.setSize(400, 400);
		panel.setLocation(112, 0);
		this.add(panel);
		
		//makes the area to the left of the ImagePanel black
		backGround.setSize(112, 400);
		backGround.setLocation(0, 0);
		this.add(backGround);
		backGround.setBackground(Color.black);
		
		//makes the area to the right of the ImagePanel black
		backGround2.setSize(112, 400);
		backGround2.setLocation(512, 0);
		this.add(backGround2);
		backGround2.setBackground(Color.black);
	}//end of Interface constructor
	
	
	/*
	 * UploadBrowser class is meant to create a JFileChooser GUI.  It implements
	 * an ActionListner that passes the path of the current selected directory
	 * into a the variable fileName so that each picture in the directory can be
	 * added to the PhotoGUI.
	 */
	public class UploadBrowser extends JFrame {
		private JFileChooser browser = new JFileChooser();
		
		public UploadBrowser(){
			//sets the JFileChooser to only display directories
			browser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
			
			//sets the properties of the JFileChooser window and adds the JFileChooser
			this.setSize(600, 400);
			this.setTitle("Open Image Directory");
			this.setResizable(false);
			this.setLocation(250, 40);
			this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
			this.add(browser);
			
			//adds an ActionListener to the buttons in the JFileChooser
			PileListener pileListen = new PileListener();
			browser.addActionListener(pileListen);
		  }//end of UploadBrowser constructor
		
		public class PileListener implements ActionListener {
			public void actionPerformed(ActionEvent arg0) {
				Object obj = arg0.getSource();
				
				//checks to see if the open button was pressed
				if(obj.toString().contains("APPROVE_OPTION")){
					
					//saves the path of the selected file as a string and sets the fileName variable equal to that string
					String s = String.valueOf(browser.getSelectedFile());
					fileName = s;
			
					//disposes of the JFileChooser and creates a PhotoGUI screen
					UploadBrowser.this.dispose();
					PhotoGUI GUI = new PhotoGUI();
					GUI.setVisible(true);
				}
				
				//checks to see if the cancel button was pressed and exits the program if it was
				if(obj.toString().contains("CANCEL")){
					System.exit(0);
				}
			}
			
		}//end of PileListener inner class
	}//end of UploadBrowser inner class
	
	public class BtnListen implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			//removes the JPanel panel from the GUI
			PhotoGUI.this.remove(panel);

			//gets the source of the ActionEvent and castes it to a button object
			JButton b = (JButton) e.getSource();
			
			//creates a new ImagePanel for the JPanel panel
			panel = new ImagePanel(b.getIcon().toString());
			panel.setSize(400, 400);
			panel.setLocation(112, 0);

			//adds the panel to the JPanel and repaints it
			PhotoGUI.this.add(panel);
			PhotoGUI.this.repaint();
		}
		
	}//end of BtnListen inner class
	
	
	/*
	 * The addImages method gets all the files in the directory specified by the
	 * variable fileName.  It then checks to see if the extension of the file is
	 * equal to a supported picture file extension.  If the file has a valid 
	 * extension, it creates a new button and adds the button to an ArrayList.
	 * Then an ActionListener is added to every button in the ArrayList.
	 */
	public void addImages(){
		if(new File(fileName).isDirectory()){
			//creats a new array of all the file names in the directory
			String [] files = new File(fileName).list();
			
			
			for(int i = 0; i < files.length; i++){
				if(!(new File(fileName + files[i]).isDirectory())){
					//gets the extension of the file
					String ext = getExtension(new File(fileName + files[i]));
					
					//checks to make sure that the file has a valid extension
					if(ext.equalsIgnoreCase("JPEG") || ext.equalsIgnoreCase("JPG") || ext.equalsIgnoreCase("GIF") || ext.equalsIgnoreCase("PNG") || ext.equalsIgnoreCase("BMP")){
						
						//creates a new imageButton object and adds it to the button arraylist
						imageButton = new JButton();
						buttons.add(imageButton);
						
						//creates a new scaled icon and sets the button's icon to the new icon created
						ImageIcon icon = (new ScaledIcon((fileName + "\\" + files[i]), 50));
						imageButton.setBackground(Color.black);
						imageButton.setIcon(icon);
					}
				}
			}
			
			//adds a button listener to every button in the ArrayList buttons
			for(int i = 0; i < buttons.size(); i++){
				BtnListen listen = new BtnListen();
				buttons.get(i).addActionListener(listen);

				thumbPan.add(buttons.get(i));
			}
		}
	}//end of addImages method
	
	
	/*
	 * Gets the extension of the File f that is passed into the method.  It stores
	 * the file name of the File f in a String, and then it gets the index of the
	 * last instance of ".".  It then returns a String containing all the
	 *characters after the ".".
	 */
	public String getExtension(File f){
		String s = f.getName();
		String ext = "";

		for(int i = s.lastIndexOf(".") + 1; i < s.length(); i++){
			ext = ext + s.charAt(i);
		}

		return ext;
	}//end of getExtension method
	
}//end of Interface class
