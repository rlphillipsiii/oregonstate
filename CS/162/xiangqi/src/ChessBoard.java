/**
 * @author
 * 
 * Chess board implementation that holds the front end GUI operations that make
 * calls into the game engine and display engine for formatting of the buttons on
 * the GUI
 */
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.UIManager;

public class ChessBoard implements ActionListener {
	/**
	 * Frame to hold the panel
	 */
	private JFrame m_frame;
	
	/**
	 * Panel to hold the button grid
	 */
	private JPanel m_panel;
	
	/**
	 * Display handler that handles formatting of the buttons in the grid
	 */
	private ChessBoardDisplay m_display;
	
	/**
	 * Game engine that holds the rules for moving pieces around
	 */
	private ChessGame m_version;
	
	/**
	 * JButtons in the grid layout
	 */
	private JButton m_buttons[][];
	
	/**
	 * Pieces on the board
	 */
	private ChessPiece m_pieces[][];
	
	/**
	 * Number of rows on the board
	 */
	private int m_rows;
	
	/**
	 * Number of columns on the board
	 */
	private int m_cols;
	
	/**
	 * Coordinates of last valid piece to be clicked.  These are set to -1
	 * when the last click was invalid or a move occurred
	 */
	private int m_rowFocused;
	private int m_colFocused;
	
	/**
	 * Initialize the chess board
	 * 
	 * @param rows
	 * 		The number of rows to display
	 * 
	 * @param cols
	 * 		The number of columns to display
	 * 
	 * @param display
	 * 		The object that will handle JButton formatting
	 * 
	 * @param version
	 * 		The game engine that holds our rule logic
	 */
	public ChessBoard(int rows, int cols, ChessBoardDisplay display, ChessGame version) 
	{
		m_rows = rows;
		m_cols = cols;
		
		m_display = display;
		m_version = version;
		
		m_rowFocused = m_colFocused = -1;
		
		/* create our JFrame with a title */
		m_frame = new JFrame();
		m_frame.setTitle("Chess Game");
		
		/* set the frame size to something reasonable */
		Toolkit tk = Toolkit.getDefaultToolkit();  
		int xSize = ((int) tk.getScreenSize().getWidth());  
		int ySize = ((int) tk.getScreenSize().getHeight());  
		
		m_frame.setBounds(xSize/2-(ySize-200)/2, 100, ySize-200, ySize-200);
		
		/* exit when we close the JFrame */
		m_frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		/* create our grid */
		m_panel = new JPanel(new GridLayout(rows, cols));
		
		m_pieces = new ChessPiece[rows][cols];
		m_buttons = new JButton[rows][cols];
		
		/* build our arrays and layout */
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				GameButton button = new GameButton(i, j);
				button.addActionListener(this);
				
				m_buttons[i][j] = button;
				m_panel.add(button);
				
				m_display.displayEmptySquare(button, i, j);
			}
		}
		
		/* add the JPanel and kill Mac's look and feel */
		m_frame.getContentPane().add(m_panel, "Center");
		
		try {
			UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
		} catch (Exception e) { }
	}
	
	public void show()
	{
		m_frame.setVisible(true);
	}
	
	/**
	 * Adds a piece to a square on the chess board
	 * 
	 * @param piece
	 * 		The piece to be added
	 * 
	 * @param row
	 * 		The row of the square
	 * 
	 * @param col
	 * 		The column of the square
	 */
	public void addPiece(ChessPiece piece, int row, int col) 
	{
		/* remove the old piece if one is already there */
		if (m_pieces[row][col] != null) {
			removePiece(row, col);
		}
		
		/* set the coordinates of the piece and add it to the array*/
		piece.setRow(row);
		piece.setColumn(col);
		m_pieces[row][col] = piece;
		
		/* update the GUI */
		JButton button = m_buttons[row][col];
		m_display.displayFilledSquare(button, piece, row, col);
	}
	
	/**
	 * Removes a piece from a square on the board
	 * 
	 * @param row
	 * 		The row of the square
	 * 
	 * @param col
	 * 		The column of the square
	 * 
	 * @return
	 * 		The piece that was removed
	 */
	public ChessPiece removePiece(int row, int col)
	{
		ChessPiece piece = m_pieces[row][col];
		
		m_pieces[row][col] = null;
		m_display.displayEmptySquare(m_buttons[row][col], row, col);
		
		return piece;
	}
	
	/**
	 * Removes all pieces from the board
	 */
	public void removeAllPieces()
	{
		/* iterate over the rows and columns */
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_cols; j++) {
				removePiece(i, j);
			}
		}
	}
	
	/**
	 * Determines whether or not a square has a piece on it
	 * 
	 * @param row
	 * 		The row of the square
	 * 
	 * @param col
	 * 		The column of the square
	 * 
	 * @return
	 * 		A boolean indicating whether or not a piece is on the square
	 */
	public boolean hasPiece(int row, int col)
	{
		/* make sure our square is in bounds */
		if (row < 0 || row >= m_rows || col < 0 || col >= m_cols) {
			return false;
		}
		
		/* empty squares have null in our pieces array */
		return (m_pieces[row][col] != null);
	}

	/**
	 * Checks to see if the piece on the given square is owned by the player
	 * whose turn it is
	 * 
	 * @param row
	 * 		The row of the square
	 * 
	 * @param col
	 * 		The column of the square
	 * 
	 * @return
	 * 		A boolean indicating if the piece is owned by the current player
	 */
	public boolean isMyPiece(int row, int col)
	{
		ChessPiece piece = m_pieces[row][col];
		if (piece == null) {
			return false;
		}
		
		return m_version.legalPieceToPlay(piece);
	}
	
	/**
	 * Removes highlighting from all the JButtons
	 */
	public void clearAllSquares()
	{
		/* loop through all rows and columns and remove all the highlighting */
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_cols; j++) {
				clearSquare(i, j);
			}
		}
	}
	
	/**
	 * Removes highlighting from the given square if there is a piece on it
	 * 
	 * @param row
	 * 		The row of the square
	 * 
	 * @param col
	 * 		The column of the square
	 */
	public void clearSquare(int row, int col)
	{
		/* nothing to clear if there is no piece */
		if (!hasPiece(row, col)) {
			return;
		}
		
		/* grab our piece and corresponding button */
		JButton button = m_buttons[row][col];
		ChessPiece piece = m_pieces[row][col];
		
		/* remove the highlighting */
		m_display.highlightSquare(false, button, piece, row, col);
	}
	
	/**
	 * Highlights the given square if there is a piece on it.
	 * 
	 * @param row
	 * 		The row of the square
	 * 
	 * @param col
	 * 		The column of the square
	 */
	public void highlightSquare(int row, int col)
	{
		/* nothing to highlight if there is no piece */
		if (!hasPiece(row, col)) {
			return;
		}
		
		/* find our piece and corresponding button */
		JButton button = m_buttons[row][col];
		ChessPiece piece = m_pieces[row][col];
		
		/* track our first click */
		m_rowFocused = row;
		m_colFocused = col;
		
		/* highlight the square */
		m_display.highlightSquare(true, button, piece, row, col);
	}
	
	/**
	 * Gets the piece on the given square
	 * 
	 * @param row
	 * 		The row of the square
	 * 
	 * @param col
	 * 		The column of the square
	 * 
	 * @return
	 * 		The piece on the given square or null if there is none
	 */
	public ChessPiece getPiece(int row, int col) 
	{
		if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
			return m_pieces[row][col];
		}
		
		return null;
	}
	
	/**
	 * Handles the click event for the JButtons
	 * 
	 * @param e
	 * 		The event that triggered the callback
	 */
	public void actionPerformed(ActionEvent e) 
	{
		/* the source of the event */
		GameButton button = (GameButton)e.getSource();
		
		/* row and column of the button that triggered the event */
		int row = button.getRow();
		int col = button.getColumn();
		
		/* same button clicked twice in a row */
		if (row == m_rowFocused && col == m_colFocused) {
			return;
		}
		
		/* highlight the piece if it's owned by the player whose turn it is */
		if (isMyPiece(row, col)) {
			clearAllSquares();
			highlightSquare(row, col);
		} else {
			/* check if this is a second click */
			ChessPiece piece = getPiece(m_rowFocused, m_colFocused);
			if (piece != null) {
				/* attempt our move if it's a second click */
				m_version.makeMove(piece, row, col);
			}
			
			/* clear our highlighting variables */
			clearSquare(m_rowFocused, m_colFocused);
			m_rowFocused = m_colFocused = -1;
		}
	}
	
	/**
	 * @author
	 *
	 * Simple extension of JButton that knows its own row and column for convenience
	 * in the actionPerformed method
	 */
	private class GameButton extends JButton {
		/**
		 * Serialization ID
		 */
		private static final long serialVersionUID = 1L;
		
		/**
		 * Row of the JButton on the GridLayout
		 */
		private int m_row;
		
		/**
		 * Column of the JButton on the GridLayout
		 */
		private int m_col;
		
		/**
		 * Initialize the JButton
		 * 
		 * @param row
		 * 		The row of the JButton
		 * 
		 * @param col
		 * 		The column of the JButton
		 */
		public GameButton(int row, int col)
		{
			m_row = row;
			m_col = col;
		}
		
		/**
		 * Gets the row of the JButton
		 * 
		 * @return
		 * 		The row of the JButton
		 */
		public int getRow()
		{
			return m_row;
		}
		
		/**
		 * Gets the column of the JButton
		 * 
		 * @return
		 * 		The column of the JButton
		 * @return
		 */
		public int getColumn()
		{
			return m_col;
		}
	}
}
