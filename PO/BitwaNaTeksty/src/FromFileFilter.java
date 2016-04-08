/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;
import java.util.*;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;


/**
 * @author Kuba
 * Implementacja filtra, pobiera listê nieistotnych wyrazów z pliku.
 */
public class FromFileFilter implements Filter {
	/**
	 * @param filename Œcie¿ka do pliku z nieistotnymi wyrazami.
	 */
	public FromFileFilter(String filename) {
		irrelevantWords = new TreeSet<String>();
		try	{
			byte[] bytes = Files.readAllBytes(Paths.get(filename));
			String text = new String(bytes, StandardCharsets.UTF_8);
			irrelevantWords.addAll(Arrays.asList(text.split("\\s")));
		}
		catch(IOException e) {
			System.err.println("Error while loading file " + filename);
		}		
	}
	
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.Filter#isRelevant(java.lang.String)
	 */
	public boolean isRelevant(String word) {
		return !irrelevantWords.contains(word);
	}

	private Set<String> irrelevantWords;
}
