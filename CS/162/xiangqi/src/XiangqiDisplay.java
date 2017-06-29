/**
 * @author 
 * 
 * Implementation of the chess board display.  It contains simple functions to style
 * the JButtons on the board
 */

import java.awt.Color;
import javax.swing.JButton;

public class XiangqiDisplay implements ChessBoardDisplay {
	
	public void displayEmptySquare(JButton button, int row, int col) 
	{
		/* clear the text and set the color to gray */
		button.setBackground(Color.GRAY);
		button.setText("");
		button.setIcon(null);
	}

	public void displayFilledSquare(JButton button, ChessPiece piece, int row, int col) 
	{		
		/* set the color and text of the JButton to the color and text of the piece */
		button.setBackground(piece.getColor());
		button.setText(piece.getLabel());
		button.setIcon(piece.getIcon());
	}

	public void highlightSquare(boolean state, JButton button, ChessPiece piece, int row, int col) 
	{
		if (state) {
			button.setBackground(Color.YELLOW);
		} else {
			if (piece == null) {
				displayEmptySquare(button, row, col);
			} else {
				displayFilledSquare(button, piece, row, col);
			}
		}
	}

}
