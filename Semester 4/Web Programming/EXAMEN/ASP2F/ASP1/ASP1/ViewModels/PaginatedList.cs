using Microsoft.EntityFrameworkCore;

namespace ASP1.ViewModels
{
    // A generic class to manage paginated lists of data.
    // It inherits from List<T> to hold the items for the current page.
    public class PaginatedList<T> : List<T>
    {
        // The current page number (1-indexed).
        public int PageIndex { get; private set; }
        // The total number of pages available.
        public int TotalPages { get; private set; }
        // The number of items per page.
        public int PageSize { get; private set; }

        // Constructor to create a paginated list.
        // Takes a list of items for the current page, total count of all items, current page index, and page size.
        public PaginatedList(List<T> items, int count, int pageIndex, int pageSize)
        {
            PageIndex = pageIndex;
            TotalPages = (int)Math.Ceiling(count / (double)pageSize); // Calculate total pages.
            PageSize = pageSize;

            this.AddRange(items); // Add the items for the current page to this list.
        }

        // Property to check if there is a previous page.
        public bool HasPreviousPage
        {
            get
            {
                return (PageIndex > 1);
            }
        }

        // Property to check if there is a next page.
        public bool HasNextPage
        {
            get
            {
                return (PageIndex < TotalPages);
            }
        }

        // Static asynchronous method to create a PaginatedList from an IQueryable source.
        // This method efficiently fetches only the items needed for the current page.
        public static async Task<PaginatedList<T>> CreateAsync(IQueryable<T> source, int pageIndex, int pageSize)
        {
            var count = await source.CountAsync(); // Get the total count of items from the source.
            // Skip items from previous pages and take only the items for the current page.
            var items = await source.Skip((pageIndex - 1) * pageSize).Take(pageSize).ToListAsync();
            return new PaginatedList<T>(items, count, pageIndex, pageSize); // Return a new PaginatedList instance.
        }
    }
} 