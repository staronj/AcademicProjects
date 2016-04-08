/**
 * Copyright Jakub Staroń, 2015
 */

public class NormalGreibachGrammar extends ContextFreeGrammar {
	public NormalGreibachGrammar(ContextFreeGrammar grammar) {
		super(grammar);
		if (!grammar.ifGreibach()) {
			throw new IllegalArgumentException("RegularGrammar: grammar is not in Chomsky form!");
		}
	}
}