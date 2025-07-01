// Wait for the page to load
$(document).ready(function() {
    // Handle text selection
    $('.content').on('mouseup', function() {
        // Clear previous highlights
        $('.highlighted').each(function() {
            $(this).replaceWith($(this).text());
        });

        // Get selected text
        let selectedText = window.getSelection().toString().trim();

        // If there's selected text, highlight matches
        if (selectedText) {
            // Create a pattern to find all selected words
            // 1. Split the selection into separate words
            // 2. Add word boundaries (\b) around each word
            let pattern = selectedText.split(/\s+/)
                .map(word => '\\b' + word + '\\b')
                .join('|');
            // Create regex with global (g) and case-insensitive (i) flags
            let regex = new RegExp(pattern, 'gi');

            // Look through all text in the content area
            $('.content').find('*').contents().filter(function() {
                // Only look at text nodes (nodeType 3), not HTML elements
                return this.nodeType === 3;
            }).each(function() {
                let text = $(this).text();
                // If this text contains any matches
                if (regex.test(text)) {
                    // Wrap each match in a highlighted span
                    // $& represents the matched text
                    $(this).replaceWith(
                        text.replace(regex, '<span class="highlighted">$&</span>')
                    );
                }
            });
        }
    });
}); 