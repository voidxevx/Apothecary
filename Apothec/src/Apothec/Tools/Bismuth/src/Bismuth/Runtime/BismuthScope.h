#pragma once

#include "../Data/DataIOStream.h"

#include <memory>

namespace bismuth::runtime
{

	class Scope
	{
	public:
		Scope(EntityID _this, Scope* last, bool downAccess, std::shared_ptr<DataIO> stream)
			: m_ScopeThis(_this)
			, m_Last(last)
			, m_DownAccess(downAccess)
			, m_IOStream(stream)
		{
			m_TemporaryData = (const void**)malloc(BISMUTH_STREAM_SIZE * sizeof(const void*));
		}

		/*
		 * Deallocate The data instances and temporary data created in this scope 
		 */
		~Scope();

		/*
		 * Creates a variable for a data ptr that can be accessed in this scope and higher scopes with down access. 
		 */
		void CreateVariable(PropertyID id, const DataPtr& value);

		/*
		 * Sets the value of a variable. 
		 */
		void SetVariable(PropertyID id, const DataPtr& value);

		/*
		 * Get the data from a variable in this scope or preceding scopes if it has down access. 
		 */
		const DataPtr GetVariable(PropertyID id) const;

		/*
		 * point the scope pointer to the last scope and delete this scope. 
		 */
		void PopScope(Scope*& scopePointer);

		/*
		 * Creates a new scope and points the pointer to it.
		 * if the this entity is left at 0 it will inherit the this of the current scope.
		 */
		void PushScope(Scope*& scopePointer, bool downAccess, EntityID _this = 0);

		inline std::shared_ptr<DataIO> GetIO() const { return m_IOStream; }
		inline EntityID GetThis() const { return m_ScopeThis; }

	private:
		std::map<PropertyID, IDataInstance*> m_ScopeVariable; // maps variable ids to their data instances.
		const void** m_TemporaryData;                         // pointers to temporary data allocated in this scope that needs to be deleted (garbage collection)
		size_t m_TemporaryDataCount = 0;                      // the amount of temporary data created. used for iteration.
		EntityID m_ScopeThis;                                 // the 'this' entity of the scope.

		std::shared_ptr<DataIO> m_IOStream;                   // input/output stream

		Scope* m_Last;                                        // the next scope below this scope.
		bool m_DownAccess;                                    // if the scope has access to the values created in previous scopes.
	};

}