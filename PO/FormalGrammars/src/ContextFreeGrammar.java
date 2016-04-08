/**
 * Copyright Jakub Staroń, 2015
 */

import java.util.*;
import java.lang.StringBuilder;
import java.util.stream.*;

public class ContextFreeGrammar {
	/*
	 * Konstruktor ContextFreeGrammar.
	 * w @param terminals i @param nonTerminals
	 * mog¹ wystêpowaæ tylko litery odpowiedniego rozmiaru, nie mog¹ siê
	 * powtarzaæ, stringi musz¹ byæ niepuste
	 * 
	 * Konstruktor dodatkowo sprawdza, czy przekazana gramatyka jest produktywna
	 * tzn. czy KA¯DY (nawet taki, którego nie da siê otrzymaæ z aksjomatu)
	 * z nieterminali jest produktywny
	 */
	public ContextFreeGrammar(String terminals, String nonTerminals, String[][] productions) {
		this();

		parseTerminals(terminals);
		parseNonTerminals(nonTerminals);
		parseProductions(nonTerminals, productions);
		if (!isProductiveGrammar()) {
			throw new IllegalArgumentException("ContextFreeGrammar: grammar is not productive!");
		}
	}
	
	/*
	 * Konstruktor kopiuj¹cy
	 */
	public ContextFreeGrammar(ContextFreeGrammar grammar)
	{
		productions = copyProductionMap(grammar.productions);
		terminalSymbols = new TreeSet<GrammarCharacter>(grammar.terminalSymbols);
		nonTerminalSymbols = new TreeSet<GrammarCharacter>(grammar.nonTerminalSymbols);
	}
	
	protected ContextFreeGrammar()
	{
		this.terminalSymbols = new TreeSet<GrammarCharacter>();
		this.nonTerminalSymbols = new TreeSet<GrammarCharacter>();
		this.productions = new TreeMap<GrammarCharacter, List<Production>>();
	}

	public boolean ifRegular() {
		return allProductionsStream()
				.allMatch((production) -> production.isLeftRegularProduction())
				|| allProductionsStream().allMatch(
						(production) -> production.isRightRegularProduction());
	}

	public boolean ifChomsky() {
		return allProductionsStream().allMatch((production) -> production.isChomskyProduction());
	}

	public boolean ifGreibach() {
		return allProductionsStream().allMatch((production) -> production.isGreibachProduction());
	}

	public String toString() {
		return String.format("Gramatyka: %s\nTerminale: %s\nNieterminale: %s\nProdukcje: \n%s",
				grammarTypeString(), terminalListString(), nonTerminalListString(),
				productionListString());
	}

	/*
	 * gramatyka jest w postaci A (A nale¿y do zbioru {Chomsky, Greibach, regularna}),
	 * gdy funkcja ifA zwraca true. W szczególnoœci gramatyka mo¿e byæ w wiêcej
	 * ni¿ jednej postaci na raz. Jeœli gramatyka nie jest w ¿adnej z wy¿ej wymienionych
	 * postaci, to uwa¿amy jej postaæ za postaæ bezkontekstow¹. Jeœli gramatyka jest w
	 * jakichœ z wy¿ej wymienionych postaci, to wszystkie one s¹ wypisywane, oddzielone
	 * znakiem '/'.
	 */
	public String grammarTypeString() {
		ArrayList<String> types = new ArrayList<String>();
		if (ifChomsky())
			types.add("Chomsky");
		if (ifGreibach())
			types.add("Greibach");
		if (ifRegular())
			types.add("regularna");

		if (types.isEmpty())
			return "bezkontekstowa";
		else
			return String.join("/", types);

	}

	public String terminalListString() {
		StringBuilder builder = new StringBuilder();
		for (GrammarCharacter ch : terminalSymbols) {
			builder.append(ch.toString());
		}
		return builder.toString();
	}

	public String nonTerminalListString() {
		StringBuilder builder = new StringBuilder();
		for (GrammarCharacter ch : nonTerminalSymbols) {
			builder.append(ch.toString());
		}
		return builder.toString();
	}

	public String productionListString() {
		StringBuilder builder = new StringBuilder();
		allProductionsStream().forEach((production) -> {
			builder.append(production.toString());
			builder.append('\n');
		});
		return builder.toString();
	}

	protected Stream<Production> allProductionsStream() {
		Stream<List<Production>> productionListStream = productions.values().stream();
		return productionListStream.flatMap((productionList) -> {
			return productionList.stream();
		});
	}

	private void parseTerminals(String terminals) {
		for (int i = 0; i < terminals.length(); i++) {
			TerminalCharacter ch = new TerminalCharacter(terminals.charAt(i));
			boolean added = terminalSymbols.add(ch);
			if (!added) {
				throw new IllegalArgumentException("Repeated characters in terminals set.");
			}
		}
	}

	private void parseNonTerminals(String nonTerminals) {
		if (nonTerminals.isEmpty())
			throw new IllegalArgumentException("Non-terminals set can not be empty!");
		for (int i = 0; i < nonTerminals.length(); i++) {
			NonTerminalCharacter ch = new NonTerminalCharacter(nonTerminals.charAt(i));
			boolean added = nonTerminalSymbols.add(ch);
			if (!added) {
				throw new IllegalArgumentException("Repeated characters in non-terminals set.");
			}
			productions.put(ch, new ArrayList<Production>());
		}
	}

	private void parseProductions(String nonTerminals, String[][] productionsStrings) {
		if (nonTerminals.length() != productionsStrings.length) {
			throw new IllegalArgumentException(
					"Number of non-terminals do not match number of productions.");
		}

		for (int i = 0; i < nonTerminals.length(); i++) {
			NonTerminalCharacter ch = new NonTerminalCharacter(nonTerminals.charAt(i));
			for (String string : productionsStrings[i]) {
				Production production = new Production(ch, parseProductionProduct(string));
				List<Production> insertedProductions = productions.get(ch);
				insertedProductions.add(production);
			}
		}
	}

	private List<GrammarCharacter> parseProductionProduct(String product) {
		List<GrammarCharacter> list = new ArrayList<GrammarCharacter>();
		for (int i = 0; i < product.length(); i++) {
			char c = product.charAt(i);
			if (TerminalCharacter.isValidTerminalCharacter(c)) {
				GrammarCharacter ch = new TerminalCharacter(c);
				if (terminalSymbols.contains(ch)) {
					list.add(ch);
				} else {
					throw new IllegalArgumentException(
							"Character from production does not occut in alphabet: " + c);
				}
			} else if (NonTerminalCharacter.isValidNonTerminalCharacter(c)) {
				GrammarCharacter ch = new NonTerminalCharacter(c);
				if (nonTerminalSymbols.contains(ch)) {
					list.add(ch);
				} else {
					throw new IllegalArgumentException(
							"Character from production does not occut in alphabet: " + c);
				}
			} else {
				throw new IllegalArgumentException("Invalid character in production: " + c);
			}
		}
		return list;
	}

	/*
	 * Funkcja sprawdzajca, czy gramatyka jest produktywna, tzn. czy ka¿dy nie-terminal
	 * jest produktywny.
	 */
	private boolean isProductiveGrammar() {
		Set<GrammarCharacter> productiveNonTerminals = new TreeSet<GrammarCharacter>();

		boolean change = true;
		while (change) {
			change = false;
			for (GrammarCharacter ch : nonTerminalSymbols) {
				if (!productiveNonTerminals.contains(ch)) {
					List<Production> chProductions = productions.get(ch);
					boolean productiveCh = chProductions.stream()
							.anyMatch(
									production -> production
											.isProductiveProduction(productiveNonTerminals));
					if (productiveCh) {
						change = true;
						productiveNonTerminals.add(ch);
					}
				}
			}
		}
		return productiveNonTerminals.size() == nonTerminalSymbols.size();
	}
	
	protected static Map<GrammarCharacter, List<Production>> copyProductionMap(Map<GrammarCharacter, List<Production>> map)
	{
		Map<GrammarCharacter, List<Production>> mapCopy = new TreeMap<GrammarCharacter, List<Production>>();;
		for(Map.Entry<GrammarCharacter, List<Production>> entry : map.entrySet())
		{
			List<Production> listCopy = new ArrayList<Production>(entry.getValue());
			mapCopy.put(entry.getKey(), listCopy);
		}
		return mapCopy;
	}

	public static void main(String[] argv) {
		{
			String[][] productions = { { "AB", "AA", "BB" }, { "a", "BB", "AA" }, { "b", "AA" } };
			ContextFreeGrammar grammar = new ContextFreeGrammar("ab", "SAB", productions);
			NormalChomskyGrammar chomsky = new NormalChomskyGrammar(grammar);
			System.out.println(chomsky.toString());
			NormalGreibachGrammar greibach = chomsky.toGreibach();
			System.out.println(greibach.toString());
		}
		{
			String[][] productions = { {"a"} };
			ContextFreeGrammar grammar = new ContextFreeGrammar("a", "S", productions);
			System.out.println(grammar.toString());
		}
		{
			String[][] productions = { { "aB", "aA", "bB" }, { "bB" }, { "b" } };
			ContextFreeGrammar grammar = new ContextFreeGrammar("ab", "SAB", productions);
			System.out.println(grammar.toString());
		}
		{//Nieproduktywna, rzuci wyj¹tek
			String[][] productions = { { "A", "B", "AB" }, { "Ba" }, { "Ab" } };
			ContextFreeGrammar grammar = new ContextFreeGrammar("ab", "SAB", productions);
			System.out.println(grammar.toString());
		}
	}

	protected Set<GrammarCharacter> terminalSymbols;
	protected Set<GrammarCharacter> nonTerminalSymbols;
	protected Map<GrammarCharacter, List<Production>> productions;
}
