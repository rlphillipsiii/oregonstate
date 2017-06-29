package com.thirdgen.chip8.test;

import static org.junit.Assert.*;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import com.thirdgen.chip8.Chip8;
import com.thirdgen.chip8.OpCode;
import com.thirdgen.chip8.Register;
import com.thirdgen.chip8.Stack;

public class OpCodeTest {
	private static Register m_memory[];
	private static Register m_V[];
	
	private static byte m_pixels[];
	private static byte m_keys[];
		
	private static Register m_Ireg;
	private static Register m_pc;

	private static Stack m_stack;
	
	private static Register m_delay;
	private static Register m_sound;
	
	@BeforeClass
	public static void setUpBeforeClass() throws Exception 
	{
		m_pixels = new byte[2048];
		
		m_memory = new Register[Chip8.CHIP8_MEMORY_COUNT];
		for (int i = 0; i < m_memory.length; i++) {
			m_memory[i] = new Register(Chip8.CHIP8_MEMORY_BLOCK_SIZE, 0, "Memory_" + Integer.toHexString(i));
		}
		
		m_V = new Register[Chip8.CHIP8_VREGISTER_COUNT];
		for (int i = 0; i < m_V.length; i++) {
			m_V[i] = new Register(Chip8.CHIP8_VREGISTER_BLOCK_SIZE, 0, "V_" + Integer.toHexString(i));
		}
		
		for (int i = 0; i < Chip8.CHIP8_FONTSET.length; i++) {
			m_memory[i].set((Chip8.CHIP8_FONTSET[i] & 0xFF));
		}

		m_Ireg = new Register(Chip8.CHIP8_IREG_SIZE, 0, "I");
		m_pc = new Register(Chip8.CHIP8_PC_SIZE, Chip8.CHIP8_ENTRY_ADDRESS, "PC");
		
		m_delay = new Register(Chip8.CHIP8_DELAY_REGISTER_SIZE, 0, "Delay");
		m_sound = new Register(Chip8.CHIP8_SOUND_REGISTER_SIZE, 0, "Sound");
		
		m_stack = new Stack();
		
		m_keys = new byte[16];
		for (int i = 0; i < m_keys.length; i++) {
			m_keys[i] = 0x00;
		}
		
		Register.enableDebugMessages(true);
		OpCode.enableDebugMessages(true);
	}

	@AfterClass
	public static void tearDownAfterClass() throws Exception 
	{
	}

	@Before
	public void setUp() throws Exception 
	{
		
	}

	@After
	public void tearDown() throws Exception 
	{
	}

	@Test
	public void testSetRegister()
	{
		System.out.println("--------- TEST REGISTER DIRECT SETS --------");
		
		int value = 0x10;
		
		/* test immediate sets */
		for (int i = 0; i < m_V.length; i++) {			
			int val = value + i;
			
			m_V[i].set(val);
			
			assertEquals(m_V[i].getValue(), val);
			assertEquals(m_V[i].compare(val - 1), Register.GREATER_THAN);
			assertEquals(m_V[i].compare(val + 1), Register.LESS_THAN);
			assertEquals(m_V[i].compare(val), Register.EQUALS);
		}
		
		/* test register sets */
		for (int i = 2; i + 2 < m_V.length; i += 2) {
			m_V[i].set(m_V[i + 1]);
			
			assertEquals(m_V[i].getValue(), value + i + 1);
			assertEquals(m_V[i].getValue(), m_V[i + 1].getValue());
			assertEquals(m_V[i].compare(m_V[i - 1]), Register.GREATER_THAN);
			assertEquals(m_V[i].compare(m_V[i + 2]), Register.LESS_THAN);
			assertEquals(m_V[i].compare(m_V[i + 1]), Register.EQUALS);
		}
	}
	
	@Test
	public void testAddRegister()
	{
		System.out.println("--------- TEST REGISTER ADDITION --------");
		
		int value1 = 0x24;
		int value2 = 0x32;
		
		int sum = value1 + value2;
		
		m_V[0].set(value1);
		m_V[1].set(value2);
		
		m_V[0].increment(value2);
		assertEquals(m_V[0].getValue(), sum);
		
		m_V[0].set(value1);
		m_V[0].increment(m_V[1]);
		assertEquals(m_V[0].getValue(), sum);
		
		value1 = 0xEA;
		m_V[0].set(value1);
		m_V[0].increment(value2);
		assertEquals(m_V[0].getValue(), value1 + value2 - 0xFF + 1);
	}
	
	@Test
	public void testSubRegister()
	{
		System.out.println("--------- TEST REGISTER SUBTRACTION --------");

		int value1 = 0xC2;
		int value2 = 0x1F;
		
		int diff = value1 - value2;
		
		m_V[0].set(value1);
		m_V[1].set(value2);
		
		m_V[0].decrement(value2);
		assertEquals(m_V[0].getValue(), diff);
		
		m_V[0].set(value1);
		m_V[0].decrement(m_V[1]);
		assertEquals(m_V[0].getValue(), diff);
		
		m_V[0].set(value2);
		m_V[0].decrement(value1);
		assertEquals(m_V[0].getValue(), 0xFF + (value2 - value1) + 1);
	}
}
