/**
 * @author 
 * 
 * Implementation of a piece that can only move in a small box 
 */
public class BoxedPiece extends ChessPiece {

	public BoxedPiece(ChessGame.Side owner, ChessBoard board, String label) {
		super(owner, board, label);
	}

	@Override
	public boolean isLegalNonCaptureMove(int row, int col) {
		/* check our bounds */
		if (col < 3 || col > 5) {
			return false;
		}
		
		if (row > 2 && m_owner == ChessGame.Side.NORTH) {
			return false;
		}
		
		if (row < 7 && m_owner == ChessGame.Side.SOUTH) {
			return false;
		}
		
		return true;
	}

	@Override
	public boolean isLegalCaptureMove(int row, int col) {
		return isLegalNonCaptureMove(row, col);
	}

}
