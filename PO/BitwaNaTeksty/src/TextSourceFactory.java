/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;

/**
 * @author Kuba
 * Interfejs fabryki Ÿróde³ tekstów.
 */
public interface TextSourceFactory {
	/**
	 * Funkcja konstruuj¹ca.
	 * @param artist Nazwa artysty.
	 * @return ród³o tekstów.
	 */
	public TextSource construct(String artist);
	
	/**
	 * Inicjuje fabrykê.
	 * @param path Œcie¿ka/url.
	 */
	public void init(String path);
	
	/**
	 * @return Czy fabryka jest zainicjalizowana.
	 */
	public boolean ready();
}
