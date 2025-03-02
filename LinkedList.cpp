#include <iostream>
#include <string>
using namespace std;

struct Website
{
	string url;
	Website* next;
	Website* prev;

	Website(const string& url) : url(url), next(nullptr), prev(nullptr) {}
};

class BrowserHistory
{
private:
	Website* current;
public:
	BrowserHistory() : current(nullptr) {}

	// Add a website to the end of the list
	void addWebsite(const string& url) {
		Website* newWebsite = new Website(url);
		if (!current)
		{
			current = newWebsite;
		}
		else
		{
			newWebsite->prev = current;
			current->next = newWebsite;
			current = newWebsite;
		}
	}

	// Go to the next website
	void goForward()
	{
		if (current && current->next)
		{
			current = current->next;
			cout << "Navigated forward to: " << current->url << endl;
		}
		else
		{
			cout << "End of the list reached." << endl;
		}
	}

	// Go to the previous website
	void goBackward()
	{
		if (current && current->prev)
		{
			current = current->prev;
			cout << "Navigated backward to: " << current->url << endl;
		}
		else
		{
			cout << "Beginning of the list reached." << endl;
		}
	}

	// Display the current website
	void displayCurrent()
	{
		if (current)
		{
			cout << "Current website: " << current->url << endl;
		}
		else
		{
			cout << "The list is empty." << endl;
		}
	}

	void displayList()
	{
		Website* temp = current;
		if (temp)
		{
			while (temp->prev)
			{
				temp = temp->prev;
			}

			while (temp)
			{
				if (!temp->next)
				{
					cout << temp->url;
				}
				else
				{
					cout << temp->url << " -> ";
				}
				temp = temp->next;
			}
			cout << endl;
		}
		else
		{
			cout << "The list is empty." << endl;
		}
	}

	// Delete the current website from the list
	void deleteCurrent()
	{
		if (current)
		{
			Website* temp = current;
			if (current->prev)
			{
				current = current->prev;
				current->next = temp->next;
				if (temp->next)
				{
					temp->next->prev = current;
				}
			}
			else
			{
				current = current->next;
				if (current)
				{
					current->prev = nullptr;
				}
			}
			delete temp;
			cout << "Current website deleted." << endl;
		}
		else
		{
			cout << "The list is empty." << endl;
		}
	}

	// Find a website in the list
	void findWebsite(const string& url)
	{
		Website* temp = current;
		while (temp->next)
		{
			temp = temp->next;
		}
		while (temp)
		{
			if (temp->url == url)
			{
				cout << "Website found: " << url << std::endl;
				return;
			}
			temp = temp->prev;
		}
		cout << "Website not found." << std::endl;
	}
};

int main() {
	BrowserHistory history;
	string url;
	int choice;

	while (true) {
		cout << "\nMenu:\n";
		cout << "1. Display the list\n";
		cout << "2. Go forward and display the webpage\n";
		cout << "3. Go backward and display the webpage\n";
		cout << "4. Add another item to the list\n";
		cout << "5. Delete an item from the list\n";
		cout << "6. Find an item in the list\n";
		cout << "7. Exit\n";
		cout << "Enter your choice: ";
		cin >> choice;


		switch (choice) {
		case 1:
			history.displayList();
			break;
		case 2:
			history.goForward();
			break;
		case 3:
			history.goBackward();
			break;
		case 4:
			cout << "Enter the URL to add: ";
			cin.ignore(); // Ignore the newline left in the input buffer
			getline(cin, url);
			history.addWebsite(url);
			break;
		case 5:
			history.deleteCurrent();
			break;
		case 6:
			cout << "Enter the URL to find: ";
			cin.ignore(); // Ignore the newline left in the input buffer
			getline(cin, url);
			history.findWebsite(url);
			break;
		case 7:
			cout << "Exiting the program.\n";
			return 0;
		default:
			cout << "Invalid choice. Try again.\n";
			break;
		}
	}

	return 0;
}