/**
 * Search ViewModel
 * Business logic for search and filtering functionality
 */

import { reactive, ref, computed } from 'vue'
import { filterService } from '@/services/FilterService.js'

export function useSearchViewModel() {
  // Reactive state
  const state = reactive({
    searchQuery: '',
    searchResults: [],
    filters: {
      patients: {
        name: '',
        ageRange: { min: null, max: null },
        gender: '',
        dateRange: { start: null, end: null },
        hasRecordings: null
      },
      recordings: {
        patientId: null,
        dateRange: { start: null, end: null },
        duration: { min: null, max: null },
        hasTranscription: null,
        confidence: { min: null, max: null }
      }
    },
    sortOptions: {
      patients: [
        { value: 'name', label: 'Nume' },
        { value: 'age', label: 'Vârsta' },
        { value: 'createdAt', label: 'Data creării' },
        { value: 'updatedAt', label: 'Ultima modificare' }
      ],
      recordings: [
        { value: 'timestamp', label: 'Data înregistrării' },
        { value: 'duration', label: 'Durata' },
        { value: 'confidence', label: 'Încrederea transcripției' }
      ]
    },
    currentSort: {
      patients: { field: 'name', order: 'asc' },
      recordings: { field: 'timestamp', order: 'desc' }
    },
    isSearching: false,
    searchHistory: [],
    recentSearches: []
  })

  // Computed properties
  const hasSearchResults = computed(() => state.searchResults.length > 0)

  const searchResultsCount = computed(() => state.searchResults.length)

  const filteredResults = computed(() => {
    return state.searchResults.filter(result => {
      // Apply additional filtering logic here if needed
      return true
    })
  })

  // Methods
  const searchPatients = async (patients, query) => {
    state.isSearching = true
    state.searchQuery = query

    try {
      let results = filterService.searchPatients(patients, query)
      results = filterService.filterPatients(results, state.filters.patients)
      results = filterService.sortPatients(
        results, 
        state.currentSort.patients.field, 
        state.currentSort.patients.order
      )

      state.searchResults = results
      addToSearchHistory(query, 'patients', results.length)

      return { success: true, results }
    } catch (error) {
      console.error('Search failed:', error)
      return { success: false, error: error.message }
    } finally {
      state.isSearching = false
    }
  }

  const searchRecordings = async (recordings, query) => {
    state.isSearching = true
    state.searchQuery = query

    try {
      let results = filterService.searchRecordings(recordings, query)
      results = filterService.filterRecordings(results, state.filters.recordings)
      results = filterService.sortRecordings(
        results, 
        state.currentSort.recordings.field, 
        state.currentSort.recordings.order
      )

      state.searchResults = results
      addToSearchHistory(query, 'recordings', results.length)

      return { success: true, results }
    } catch (error) {
      console.error('Search failed:', error)
      return { success: false, error: error.message }
    } finally {
      state.isSearching = false
    }
  }

  const searchGlobal = async (allData, query) => {
    state.isSearching = true
    state.searchQuery = query

    try {
      const results = {
        patients: [],
        recordings: [],
        total: 0
      }

      // Search patients
      if (allData.patients) {
        results.patients = filterService.searchPatients(allData.patients, query)
      }

      // Search recordings
      if (allData.recordings) {
        results.recordings = filterService.searchRecordings(allData.recordings, query)
      }

      results.total = results.patients.length + results.recordings.length
      state.searchResults = results

      addToSearchHistory(query, 'global', results.total)

      return { success: true, results }
    } catch (error) {
      console.error('Global search failed:', error)
      return { success: false, error: error.message }
    } finally {
      state.isSearching = false
    }
  }

  const setFilters = (category, filters) => {
    if (state.filters[category]) {
      state.filters[category] = { ...state.filters[category], ...filters }
    }
  }

  const setSorting = (category, field, order) => {
    if (state.currentSort[category]) {
      state.currentSort[category] = { field, order }
    }
  }

  const clearFilters = (category = null) => {
    if (category) {
      if (state.filters[category]) {
        // Reset category-specific filters
        const defaultFilters = {
          patients: {
            name: '',
            ageRange: { min: null, max: null },
            gender: '',
            dateRange: { start: null, end: null },
            hasRecordings: null
          },
          recordings: {
            patientId: null,
            dateRange: { start: null, end: null },
            duration: { min: null, max: null },
            hasTranscription: null,
            confidence: { min: null, max: null }
          }
        }
        state.filters[category] = defaultFilters[category]
      }
    } else {
      // Clear all filters
      filterService.clearFilters()
      state.filters = {
        patients: {
          name: '',
          ageRange: { min: null, max: null },
          gender: '',
          dateRange: { start: null, end: null },
          hasRecordings: null
        },
        recordings: {
          patientId: null,
          dateRange: { start: null, end: null },
          duration: { min: null, max: null },
          hasTranscription: null,
          confidence: { min: null, max: null }
        }
      }
    }
  }

  const clearSearch = () => {
    state.searchQuery = ''
    state.searchResults = []
  }

  const addToSearchHistory = (query, type, resultCount) => {
    if (query.trim()) {
      const searchEntry = {
        query: query.trim(),
        type,
        resultCount,
        timestamp: new Date().toISOString()
      }

      state.searchHistory.unshift(searchEntry)
      
      // Keep only last 20 searches
      if (state.searchHistory.length > 20) {
        state.searchHistory = state.searchHistory.slice(0, 20)
      }

      // Update recent searches (last 5)
      state.recentSearches = state.searchHistory.slice(0, 5)
    }
  }

  const getSearchSuggestions = (query, data) => {
    if (!query || query.length < 2) return []

    const suggestions = []
    const lowerQuery = query.toLowerCase()

    // Patient name suggestions
    if (data.patients) {
      data.patients.forEach(patient => {
        if (patient.name.toLowerCase().includes(lowerQuery)) {
          suggestions.push({
            type: 'patient',
            text: patient.name,
            id: patient.id,
            category: 'Pacienți'
          })
        }
      })
    }

    // Medical term suggestions
    const medicalTerms = [
      'dureri', 'durere', 'febra', 'temperatura', 'presiune', 'tensiune',
      'inima', 'puls', 'respiratie', 'oxigen', 'sange', 'test', 'analiza',
      'medicament', 'tratament', 'diagnostic', 'simptom', 'boala'
    ]

    medicalTerms.forEach(term => {
      if (term.toLowerCase().includes(lowerQuery)) {
        suggestions.push({
          type: 'term',
          text: term,
          category: 'Termeni medicali'
        })
      }
    })

    // Remove duplicates and limit results
    const uniqueSuggestions = suggestions.filter((suggestion, index, self) =>
      index === self.findIndex(s => s.text === suggestion.text)
    )

    return uniqueSuggestions.slice(0, 10)
  }

  const getFilterOptions = (data) => {
    return {
      patients: filterService.getPatientFilterOptions(data.patients || []),
      recordings: {
        durationRanges: [
          { value: { min: 0, max: 30 }, label: 'Sub 30 secunde' },
          { value: { min: 30, max: 60 }, label: '30-60 secunde' },
          { value: { min: 60, max: 300 }, label: '1-5 minute' },
          { value: { min: 300, max: null }, label: 'Peste 5 minute' }
        ],
        confidenceRanges: [
          { value: { min: 0.9, max: 1.0 }, label: 'Foarte înaltă (90-100%)' },
          { value: { min: 0.8, max: 0.9 }, label: 'Înaltă (80-90%)' },
          { value: { min: 0.7, max: 0.8 }, label: 'Medie (70-80%)' },
          { value: { min: 0, max: 0.7 }, label: 'Scăzută (sub 70%)' }
        ]
      }
    }
  }

  const exportSearchResults = async (format = 'json') => {
    try {
      const data = {
        searchQuery: state.searchQuery,
        timestamp: new Date().toISOString(),
        results: state.searchResults,
        filters: state.filters
      }

      if (format === 'json') {
        const blob = new Blob([JSON.stringify(data, null, 2)], { type: 'application/json' })
        const url = URL.createObjectURL(blob)
        const link = document.createElement('a')
        link.href = url
        link.download = `search_results_${new Date().toISOString().split('T')[0]}.json`
        document.body.appendChild(link)
        link.click()
        document.body.removeChild(link)
        URL.revokeObjectURL(url)
      }

      return { success: true }
    } catch (error) {
      return { success: false, error: error.message }
    }
  }

  return {
    // State
    searchQuery: computed(() => state.searchQuery),
    searchResults: computed(() => state.searchResults),
    filters: computed(() => state.filters),
    sortOptions: computed(() => state.sortOptions),
    currentSort: computed(() => state.currentSort),
    isSearching: computed(() => state.isSearching),
    searchHistory: computed(() => state.searchHistory),
    recentSearches: computed(() => state.recentSearches),

    // Computed
    hasSearchResults,
    searchResultsCount,
    filteredResults,

    // Methods
    searchPatients,
    searchRecordings,
    searchGlobal,
    setFilters,
    setSorting,
    clearFilters,
    clearSearch,
    getSearchSuggestions,
    getFilterOptions,
    exportSearchResults
  }
}

