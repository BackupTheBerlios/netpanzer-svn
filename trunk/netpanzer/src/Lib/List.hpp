/*
Copyright (C) 1998 Pyrosoft Inc. (www.pyrosoftgames.com), Matthew Bogue

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

template <class T>
class List;

template <class T>
class ListNode
{
  friend List<T>;

  private:
		T item;
    ListNode *next;
    ListNode *prev;

	ListNode(T item, ListNode<T> *next = NULL)
	{
		ListNode::item = item;
		ListNode::next = next;
		ListNode::prev = NULL;

		if (ListNode::next != NULL)
		{
			ListNode::next->prev = this;
		}
	}
};

template <class T>
class List
{
  private:
    ListNode<T> *head;
		int numItems;
  public:
		List();
		~List();
		int getNumItems();
		void add(T item);
		T removeFirstItem();
		T getItem(int index );
};

template <class T>
List<T>::List()
{
	head     = NULL;
	numItems = 0;
}

template <class T>
List<T>::~List()
{
	while (numItems > 0
	{
		removeFirstItem();
	}
}

template <class T>
int List<T>::getNumItems()
{
	return numItems;
}

template <class T>
void List<T>::add(T item)
{
	head = new ListNode<T>(item,head);
	numItems++;
}

template <class T>
T List<T>::removeFirstItem()
{
	if (numItems > 0)
	{
		ListNode<T> *newHead = head->next;
		T item = head->item;
		delete head;
		head = newHead;
		numItems--;
		return item;
	}
	return NULL;
}

template <class T>
T List<T>::removeItem()
{
	if (numItems > 0)
	{
		ListNode<T> *newHead = head->next;
		T item = head->item;
		delete head;
		head = newHead;
		numItems--;
		return item;
	}
	return NULL;
}
{
	if (string != NULL)
	{
		::free(string);
	}

	if (prev != NULL) prev->next = next;
	if (next != NULL) next->prev = prev;

	prev = next = this;
}

template <class T>
T List<T>::getItem(int index)
{
	if (index < 0 || index >= numItems)
		return NULL;

	ListNode<T> *node = head;
	int count = 0;

	while (count++ < index)
		node = node->next;

	return node->item;
}
