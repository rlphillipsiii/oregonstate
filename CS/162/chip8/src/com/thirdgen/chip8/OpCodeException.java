package com.thirdgen.chip8;

public class OpCodeException extends Exception {
	private static final long serialVersionUID = 1L;
	
	public OpCodeException(String message)
	{
		super(message);
	}
}
