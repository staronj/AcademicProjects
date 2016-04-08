/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;
import java.util.stream.*;

/**
 * @author Kuba
 * Abstrakcyjna klasa Ÿród³a tekstu.
 */
public abstract class TextSource {
	/**
	 * Zwraca strumieñ wyrazów.
	 * @return Strumieñ wyrazów.
	 */
	public Stream<String> wordsStream() {
		Stream<Song> songs = songsStream();
		return songs.flatMap(song -> song.wordsStream()).filter(str -> !str.isEmpty()).map(str -> str.toLowerCase());		
	}
	
	/**
	 * Zwraca nazwê artysty.
	 * @return Nazwa arytsty.
	 */
	public abstract String getArtistName();	
	
	/**
	 * Zwraca strumieñ piosenek (ale to poetycko brzmi).
	 * @return Strumieñ piosenek.
	 */
	public abstract Stream<Song> songsStream();
}
