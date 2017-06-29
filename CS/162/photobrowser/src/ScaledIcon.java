import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

/**
 * The default behavior of ImageIcon is to display the image at full resolution,
 * however, when using the image for a button, it is much more convenient if we
 * have Java scale the image instead of scaling it in advance. This class
 * creates an icon that is no larger then the specified size regardless of the
 * original image size.
 * 
 * @author Christoph Neumann
 */
public class ScaledIcon extends ImageIcon {

	/**
	 * Creates an icon that has been scaled to the specified size in pixels.
	 * This means the length of the longest size will equal to the given size.
	 * 
	 * @param path
	 *            path to the image file to load
	 * @param size
	 *            The size (in pixels) which should contain the image. For
	 *            example, a size of 50 will scale the image to fit within a
	 *            50x50 square.
	 */
	public ScaledIcon(String path, int size) {
		// Call the parent's constructor
		super(path);

		try {
			// Get the unscaled image and calculate the scale factors.
			Image img = ImageIO.read(new File(path));
			if ( img != null ) {
				double width_scale = (double) size / img.getWidth(null);
				double height_scale = (double) size / img.getHeight(null);
				double scale = width_scale;
				if (height_scale < width_scale) {
					scale = height_scale;
				}
				int new_width = (int) (img.getWidth(null) * scale);
				int new_height = (int) (img.getHeight(null) * scale);
	
				// Create a scaled version of the image in memory
				BufferedImage scaled = new BufferedImage(new_width, new_height,	BufferedImage.TYPE_INT_RGB);
				Graphics2D g2 = scaled.createGraphics();
				g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
				g2.drawImage(img, 0, 0, new_width, new_height, null);
				g2.dispose();
	
				// Replace the current image with the scaled image.
				setImage(scaled);
			} else {
				System.err.println("Error: Not an image file: "+ path);
			}
		} catch (IOException e) {
			System.err.println("Error loading image: "+ path +" "+ e.getMessage());
			e.printStackTrace();
		}
	}

}
