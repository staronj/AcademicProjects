/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;
import java.util.Collection;

/**
 * @author Kuba
 * Interfejs procesora tekstów.
 */
public interface Processor {
	/**
	 * Procesuje kolekcjê Ÿróde³ tekstów (artystów).
	 * @param sources
	 */
	public void process(Collection<TextSource> sources);
	/**
	 * Procesuje pojedyñcze Ÿród³o tekstów (pojedyñczego artystê).
	 * @param source
	 */
	public void process(TextSource source);
}
