/**
 * Copyright Jakub Staroń, 2015
 */

public abstract class GrammarCharacter implements Comparable<GrammarCharacter> {

	public String toString() {
		return Character.toString(character);
	}

	public char getChar() {
		return character;
	}

	/*
	 * Funkcja pozwalaj¹ca rozstrzygaæ, czy dany znak jest terminalny czy
	 * nieterminalny
	 */
	public abstract boolean isTerminal();

	/*
	 * Prze³adowujemy funkcje potrzebne do u¿ywania GrammarCharacter w
	 * kolekcjach typu TreeSet, HashSet
	 */
	@Override
	public boolean equals(Object aThat) {
		if ( this == aThat ) return true;
	    if ( !(aThat instanceof GrammarCharacter) ) return false;
	    GrammarCharacter that = (GrammarCharacter)aThat;
	    return this.character == that.character;
	}

	public int hashCode() {
		return Character.hashCode(character);
	}

	public int compareTo(GrammarCharacter c) {
		return Character.compare(character, c.character);
	}

	protected char character;
}
