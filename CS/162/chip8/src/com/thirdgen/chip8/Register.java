package com.thirdgen.chip8;

public class Register {
	public static final int GREATER_THAN = 1;
	public static final int LESS_THAN    = -1;
	public static final int EQUALS       = 0;
	
	private static final int DEFAULT_REG_BITS = 32;
	private static final String DEFAULT_REG_NAME = "Default";
	
	private static boolean g_debug = false;
	
	String m_name;
	long m_max;
	long m_value;
	
	public Register(int bits)
	{
		m_value = 0;
		m_max   = (long)(Math.pow(2, bits) - 1);
		m_name  = DEFAULT_REG_NAME;
	}
	
	public Register(int bits, long value, String name)
	{
		m_value = value;
		m_max   = (long)(Math.pow(2, bits) - 1);
		m_name  = name;
	}
	
	public static void enableDebugMessages(boolean debug)
	{
		g_debug = debug;
	}
	
	public long getValue()
	{
		return m_value;
	}
	
	public int compare(long value) 
	{
		return compare(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	public int compare(Register reg)
	{
		if (m_value > reg.getValue()) {
			return Register.GREATER_THAN;
		}
		if (m_value < reg.getValue()) {
			return Register.LESS_THAN;
		}
		return Register.EQUALS;
	}
	
	public void increment(Register reg)
	{
		increment(reg.getValue());
	}
	public void increment(long value)
	{
		set(add(value));
	}
	
	public void decrement(Register reg)
	{
		decrement(reg.getValue());
	}
	public void decrement(long value)
	{
		set(sub(value));
	}
	
	public long add(long value)
	{
		return add(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	public long add(Register reg)
	{
		return wrap(m_value + reg.getValue());
	}
	
	public long sub(Register reg)
	{
		return wrap(m_value - reg.getValue());
	}
	public long sub(long value)
	{
		return sub(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	
	public void andE(Register reg)
	{
		set(and(reg));
	}
	public long and(long value)
	{
		return and(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	public long and(Register reg)
	{
		return wrap(m_value & reg.getValue());
	}
	
	public void orE(Register reg)
	{
		set(or(reg));
	}
	public long or(long value)
	{
		return or(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	public long or(Register reg)
	{
		return wrap(m_value | reg.getValue());
	}
	
	public void xorE(Register reg)
	{
		set(xor(reg));
	}
	public long xor(long value)
	{
		return xor(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	public long xor(Register reg)
	{
		return wrap(m_value ^ reg.getValue());
	}
	
	public void set(Register reg)
	{
		set(reg.getValue());
	}
	public void set(long value)
	{
		m_value = value;
		wrap();
		
		if (g_debug) {
			System.out.println("Register " + m_name + " => 0x" + Integer.toHexString((int)m_value));
		}
	}
	
	public void shiftRight(long value)
	{
		shiftRight(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	public void shiftRight(Register reg)
	{
		m_value = (m_value >> reg.getValue()) & m_max;
	}
	
	public void shiftLeft(long value)
	{
		shiftLeft(new Register(DEFAULT_REG_BITS, value, DEFAULT_REG_NAME));
	}
	public void shiftLeft(Register reg)
	{
		m_value = (m_value << reg.getValue()) & m_max;
	}
	
	private long wrap(long value)
	{
		value = (value > m_max) ? (value - m_max - 1) : value;
		value = (value < 0) ? (m_max + value + 1) : value;
		
		return value;
	}
	
	private void wrap()
	{
		m_value = wrap(m_value);
	}
}
