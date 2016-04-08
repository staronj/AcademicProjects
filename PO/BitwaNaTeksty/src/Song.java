/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;
import java.util.stream.*;
import java.util.Arrays;


/**
 * @author Kuba
 * Klasa piosenki.
 */
public class Song {
	/**
	 * Konstruktor.
	 * @param title Tytu³ piosenki.
	 * @param text Tekst piosenki.
	 */
	public Song(String title, String text) {
		this.title = title;
		this.text = text;
	}
	
	/**
	 * Zwraca tekst piosenki.
	 * @return Tekst piosenki.
	 */
	public String getText() {
		return text;
	}
	
	/**
	 * Zwraca tytu³ piosenki.
	 * @return Tytu³ piosenki.
	 */
	public String getTitle() {
		return title;
	}
	
	/**
	 * @return Strumieñ wyrazów wystêpuj¹cych w piosence.
	 */
	public Stream<String> wordsStream() {
		return Arrays.stream(getText().split("[\\s,.:;?!()-]+"));
	}	
	
	private String title;
	private String text;
}
