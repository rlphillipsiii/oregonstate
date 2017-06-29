/**
 * @author
 *
 * Implementation of the chess game that builds a chess board of the correct
 * size for Xiangqi and starts the game engine.
 */
public class Xiangqi implements ChessGame {
	/**
	 * Number of rows on the Xiangqi board
	 */
	private static final int BOARD_ROWS = 10;
	
	/**
	 * Number of columns on the Xiangqi board
	 */
	private static final int BOARD_COLS = 9;
	
	/**
	 * The display that is used to handle button formatting
	 */
	private XiangqiDisplay m_display;
	
	/**
	 * The chess board that the players are playing with
	 */
	private ChessBoard m_board;
	
	/**
	 * The current side (the player whose turn it is)
	 */
	private Side m_side;
	
	/**
	 * Initializes the chess game.
	 */
	public Xiangqi()
	{
		m_display = new XiangqiDisplay();

		m_side = Side.SOUTH;
		
		m_board = new ChessBoard(BOARD_ROWS, BOARD_COLS, m_display, this);
		initBoard(m_board);
	}
	
	/**
	 * Places the pieces on the chess board.
	 * 
	 * @param board
	 * 		The board that the pieces should be added to
	 */
	private void initBoard(ChessBoard board)
	{
		/* add the north player's back line */
		board.addPiece(new RookPiece(Side.NORTH, board), 0, 0);
		board.addPiece(new KnightPiece(Side.NORTH, board), 0, 1);
		board.addPiece(new ElephantPiece(Side.NORTH, board), 0, 2);
		board.addPiece(new GuardPiece(Side.NORTH, board), 0, 3);
		board.addPiece(new KingPiece(Side.NORTH, board), 0, 4);
		board.addPiece(new GuardPiece(Side.NORTH, board), 0, 5);
		board.addPiece(new ElephantPiece(Side.NORTH, board), 0, 6);
		board.addPiece(new KnightPiece(Side.NORTH, board), 0, 7);
		board.addPiece(new RookPiece(Side.NORTH, board), 0, 8);

		/* add the cannons for the north player */
		board.addPiece(new CannonPiece(Side.NORTH, board), 2, 1);
		board.addPiece(new CannonPiece(Side.NORTH, board), 2, 7);
		
		/* add pawns every other column on rows 3 and 6 */
		for (int i = 0; i < BOARD_COLS; i += 2) {
			board.addPiece(new PawnPiece(Side.NORTH, board) , 3, i);
			board.addPiece(new PawnPiece(Side.SOUTH, board) , 6, i);
		}
		
		/* add the cannons for the south player */
		board.addPiece(new CannonPiece(Side.SOUTH, board), 7, 1);
		board.addPiece(new CannonPiece(Side.SOUTH, board), 7, 7);
		
		/* add the south player's back line */
		board.addPiece(new RookPiece(Side.SOUTH, board), 9, 0);
		board.addPiece(new KnightPiece(Side.SOUTH, board), 9, 1);
		board.addPiece(new ElephantPiece(Side.SOUTH, board), 9, 2);
		board.addPiece(new GuardPiece(Side.SOUTH, board), 9, 3);
		board.addPiece(new KingPiece(Side.SOUTH, board), 9, 4);
		board.addPiece(new GuardPiece(Side.SOUTH, board), 9, 5);
		board.addPiece(new ElephantPiece(Side.SOUTH, board), 9, 6);
		board.addPiece(new KnightPiece(Side.SOUTH, board), 9, 7);
		board.addPiece(new RookPiece(Side.SOUTH, board), 9, 8);
	}
	
	/**
	 * Gets the board being played on
	 * 
	 * @return
	 * 		The board in use
	 */
	public ChessBoard getBoard()
	{
		return m_board;
	}
	
	/**
	 * Shows the JFrame
	 */
	public void play()
	{
		m_board.show();
	}
	
	public boolean legalPieceToPlay(ChessPiece piece) 
	{
		/* true if the piece belongs to the player whose turn it is */
		return (m_side == piece.getSide());
	}
	
	public boolean makeMove(ChessPiece piece, int row, int col) 
	{		
		/* return false if the move is illegal */
		if (!piece.isLegalMove(row, col)) {
			return false;
		}
		
		/* move the piece */
		m_board.removePiece(piece.getRow(), piece.getColumn());
		m_board.addPiece(piece, row, col);
		piece.moveDone();

		/* switch the turn */
		m_side = (m_side == Side.NORTH) ? Side.SOUTH : Side.NORTH; 
		return true;
	}
	
	/**
	 * Entry point for the program.
	 * 
	 * @param args
	 * 		Unused command line arguments
	 */
	public static void main(String[] args) 
	{
		/* create a new instance of the game and play it */
		Xiangqi game = new Xiangqi();
		game.play();
	}
}
