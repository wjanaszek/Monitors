# Monitors
Simple app using monitors

Aplikacja ma zapewnić dostęp do współdzielonych zasobów, na takich samych warunkach, jak w przypadku projektu dotyczącego semaforów:
- proces A1 produkuje parzyste liczby do bufora B1, dopóki nie ma w nim X elementów
- proces A2 pobiera liczby z bufora B1 i wkłada o jeden większe do B2, dopóki w B2 jest mniej niż Y elementów
- proces A3 pobiera liczby z bufora B2 (dopóki jest w nim conajmniej Z elementów), i jeśli pobrana liczba jest podzielna przez 3, to wkłada ją do bufora B1
