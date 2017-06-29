
public class PhotoBrowser {

	/**
	
	 * @author:  Robert Phillips III
	 */
	public static void main(String[] args) {		
		PhotoGUI i = new PhotoGUI();
		PhotoGUI.UploadBrowser browse = i.new UploadBrowser();
		browse.setVisible(true);
	}
}
