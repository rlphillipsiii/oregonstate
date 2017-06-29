/**
 * @author 
 * 
 * Generic abstract class that handles common functionality for chess pieces
 */

import java.awt.Color;
import javax.swing.Icon;

public abstract class ChessPiece {
	/**
	 * Color for a JButton that this piece is placed on
	 */
	private Color m_color;
	
	/**
	 * The text label for a JButton that this piece is placed on
	 */
	private String m_label;
	
	/**
	 * The player that owns this piece
	 */
	protected ChessGame.Side m_owner;
	
	/**
	 * The chess board that this piece is on
	 */
	protected ChessBoard m_board;
	
	/**
	 * The row that this piece is on
	 */
	protected int m_row;
	
	/**
	 * The column that this piece is on
	 */
	protected int m_col;
	
	/**
	 * Checks to see if a non capture move to the given square is legal
	 * 
	 * @param row
	 * 		The row the piece is to be moved to
	 * 
	 * @param col
	 * 		The column the piece is to be moved to
	 * 
	 * @return
	 * 		A boolean indicating whether or not the move is legal
	 */
	public abstract boolean isLegalNonCaptureMove(int row, int col);
	
	/**
	 * Checks to see if a capture move to the given square is legal
	 * 
	 * @param row
	 * 		The row the piece is to be moved to
	 * 
	 * @param col
	 * 		The column the piece is to be moved to
	 * 
	 * @return
	 * 		A boolean indicating whether or not the move is legal
	 */
	public abstract boolean isLegalCaptureMove(int row, int col);
	
	/**
	 * Initializes a chess piece
	 * 
	 * @param owner
	 * 		The player that owns the piece
	 * 
	 * @param board
	 * 		The board the piece is on
	 * 
	 * @param label
	 * 		The text that a JButton should display when the piece is on it
	 */
	public ChessPiece(ChessGame.Side owner, ChessBoard board, String label)
	{
		m_owner = owner;
		m_label = label;
		m_board = board;
		
		/* north player has red pieces and south has white */
		if (owner == ChessGame.Side.NORTH) {
			m_color = Color.RED;
		} else if (owner == ChessGame.Side.SOUTH) {
			m_color = Color.WHITE;
		}
	}
	
	/**
	 * Determines the number of pieces in the path of the requested move
	 * 
	 * @param destRow
	 * 		The destination row of the piece
	 * 
	 * @param destCol
	 * 		The destination column of the piece
	 * 
	 * @return
	 * 		The number of pieces in the destination path
	 */
	public int piecesInPath(int destRow, int destCol)
	{
		if (destRow == m_row) {
			return piecesInHorzPath(destCol);
		} else if (destCol == m_col) {
			return piecesInVertPath(destRow);
		} else {
			return piecesInDiagPath(destRow, destCol);
		}
	}
	
	/**
	 * Determines the number of pieces in the diagonal path of the requested move
	 * 
	 * @param destRow
	 * 		The destination row of the piece
	 * 
	 * @param destCol
	 * 		The destination column of the piece
	 * 
	 * @return
	 * 		The number of pieces in the destination path
	 */
	public int piecesInDiagPath(int destRow, int destCol) 
	{
		/* determine if the piece is going up or down */
		int rowInc = (m_row < destRow) ? 1 : -1;
		
		/* determine if the piece is going left or right */
		int colInc = (m_col < destCol) ? 1 : -1;
		
		/* move one square diagonally */
		int row = m_row+rowInc;
		int col = m_col+colInc;
		
		int pieces = 0;
		do {
			/* check if there is a piece on the square */
			if (m_board.hasPiece(row, col)) {
				pieces++;
			}
			
			/* move to the next diagonal square */
			row += rowInc;
			col += colInc;
			
			/* kick the loop if we tested all our squares */
		} while (row != destRow+rowInc && col != destCol+colInc);
		
		return pieces;
	}
	
	/**
	 * Determines the number of pieces in the vertical path of the requested move
	 * 
	 * @param destRow
	 * 		The destination row of the piece
	 * 
	 * @return
	 * 		The number of pieces in the destination path
	 */
	public int piecesInVertPath(int destRow)
	{
		/* number of pieces in the path */
		int pieces = 0;
		
		/* figure out which square is farther left */
		int start = m_row+1;
		int end = destRow;
		
		if (start > end) {
			start = destRow;
			end = m_row-1;
		}
		
		/* iterate over the squares from left to right */
		for (int i = start; i <= end; i++) {
			/* check if there are pieces in our path */
			if (m_board.hasPiece(i, m_col)) {
				pieces++;
			}
		}
		
		return pieces;
	}
	
	/**
	 * Determines the number of pieces in the horizontal path of the requested move
	 * 
	 * @param destCol
	 * 		The destination column of the piece
	 * 
	 * @return
	 * 		The number of pieces in the destination path
	 */
	public int piecesInHorzPath(int destCol)
	{
		/* number of pieces in the path */
		int pieces = 0;
		
		/* figure out which square is higher on the board */
		int start = m_col+1;
		int end = destCol;
		
		if (start > end) {
			start = destCol;
			end = m_col-1;
		}
		
		/* iterate over the path */
		for (int i = start; i <= end; i++) {
			/* count the pieces in our path */
			if (m_board.hasPiece(m_row, i)) {
				pieces++;
			}
		}
		
		return pieces;
	}
	
	/**
	 * Determines whether the requested move is legal or not
	 * 
	 * @param row
	 * 		The destination row
	 * 
	 * @param col
	 * 		The destination column
	 * 
	 * @return
	 * 		A boolean indicating whether the move is legal or not
	 */
	public boolean isLegalMove(int row, int col)
	{
		/* 
		 * check for a non capture move if our destination doesn't have a piece on it
		 * otherwise we'll check for the legality of a capture move
		 */
		if (m_board.hasPiece(row, col)) {
			return isLegalCaptureMove(row, col);
		} else {
			return isLegalNonCaptureMove(row, col);
		}
	}
	
	/**
	 * Determines if the requested move is a diagonal move
	 * 
	 * @param row
	 * 		The destination row
	 * 
	 * @param col
	 * 		The destination column
	 * 
	 * @return
	 * 		A boolean indicating whether or not the requested move is diagonal
	 */
	public boolean isMovingDiagonally(int row, int col)
	{
		/* if neither matches then we must be going diagonally */
		return (m_row != row && m_col != col);
	}
	
	/**
	 * Determines if the requested move is a straight diagonal move
	 * 
	 * @param row
	 * 		The destination row
	 * 
	 * @param col
	 * 		The destination column
	 * 
	 * @return
	 * 		A boolean indicating whether or not the requested move is straight diagonal
	 */
	public boolean isMovingStraightDiagonally(int row, int col)
	{
		/* displacement in X and Y must be equal for straight diagonals */
		return (Math.abs(row-m_row) == Math.abs(col-m_col));
	}
	
	/**
	 * Sets the row of the piece
	 * 
	 * @param row
	 * 		The new row value
	 */
	public void setRow(int row)
	{
		m_row = row;
	}
	
	/**
	 * Sets the column of the piece
	 * 
	 * @param col
	 * 		The new column
	 */
	public void setColumn(int col)
	{
		m_col = col;
	}
	
	/**
	 * Gets the row of the piece
	 * 
	 * @return
	 * 		The row of the piece on the chess board
	 */
	public int getRow()
	{
		return m_row;
	}
	
	/**
	 * Gets the column of the piece
	 * 
	 * @return
	 * 		The column of the piece on the chess board
	 */
	public int getColumn()
	{
		return m_col;
	}
	
	/**
	 * Gets the side that owns the piece
	 * 
	 * @return
	 * 		The side of the piece
	 */
	public ChessGame.Side getSide()
	{
		return m_owner;
	}
	
	/**
	 * Gets the color of the piece
	 * 
	 * @return
	 * 		The color of the piece
	 */
	public Color getColor()
	{
		return m_color;
	}
	
	/**
	 * Gets the label of the piece
	 * 
	 * @return
	 * 		The label of the piece
	 */
	public String getLabel()
	{
		return m_label;
	}
	
	/**
	 * Gets the icon of the piece (always returns null)
	 * 
	 * @return
	 * 		null
	 */
	public Icon getIcon()
	{
		return null;
	}
	
	/**
	 * Gets the chess board that the piece is on
	 * 
	 * @return
	 * 		The board holding the piece
	 */
	public ChessBoard getChessBoard()
	{
		return m_board;
	}
	
	/**
	 * No-op
	 */
	public void moveDone()
	{
	}
}
