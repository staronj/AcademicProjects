/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;

/**
 * @author Kuba
 * Interfejs filtra wyrazów.
 */
public interface Filter {
	/**
	 * Rozstrzyga, czy wyraz jest istotny.
	 * @param word Wyraz.
	 * @return Czy jest istotny.
	 */
	public boolean isRelevant(String word);
}
