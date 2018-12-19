#ifndef ABSTRACTIONS_DATA_SET_HPP
#define ABSTRACTIONS_DATA_SET_HPP

namespace abstractions
{
    
    namespace data 
    {
    
        namespace set
        {
            
            namespace definition
            {
                    
                template <typename A>
                struct existence {
                    bool empty(A a) const {
                        return a.empty();
                    }
                };
                    
                template <typename A>
                struct existence<A*> {
                    bool empty(A a) const {
                        return a == nullptr;
                    }
                };

                template <typename A>
                struct existence<pointer<A>> {
                    bool empty(A a) const {
                        return a == nullptr;
                    }
                };
                    
                template <typename A, typename X>
                struct set : public existence<A> {
                    bool contains(A a, X x) const {
                        return a.contains(x);
                    }
                };
                    
                template <typename A, typename X>
                struct set<A*, X> : public existence<A*> {
                    bool contains(A a, X x) const {
                        if (a == nullptr) return false;
                        return a->contains(x);
                    }
                };
                    
                template <typename A, typename X>
                struct set<pointer<A>, X> : public existence<pointer<X>> {
                    bool contains(A a, X x) const {
                        if (a == nullptr) return false;
                        return a->contains(x);
                    }
                };
                    
                template <typename A, typename X>
                struct removable : public set<A, X> {
                    A remove(A a, X x) const {
                        return a.remove(x);
                    }
                };
                    
                template <typename A, typename X>
                struct removable<A*, X> : public set<A*, X> {
                    A remove(A a, X x) const {
                        if (a == nullptr) return nullptr;
                        return a->remove(x);
                    }
                };
                    
                template <typename A, typename X>
                struct removable<pointer<A>, X> : public set<pointer<A>, X> {
                    A remove(A a, X x) const {
                        if (a == nullptr) return nullptr;
                        return a->remove(x);
                    }
                };
                    
                template <typename A, typename X>
                struct insertable : public set<A, X> {
                    A add(A a, X x) const {
                        return a.add(x);
                    }
                        
                    A plus(A a, X x) {
                        return a + x;
                    }
                };
                    
                template <typename A, typename X>
                struct insertable<A*, X> : public set<A*, X> {
                    A add(A a, X x) const {
                        if (a == nullptr) return nullptr;
                        return a->add(x);
                    }
                };
                    
                template <typename A, typename X>
                struct insertable<pointer<A>, X> : public set<pointer<A>, X> {
                    A add(A a, X x) const {
                        if (a == nullptr) return nullptr;
                        return a->add(x);
                    }
                };

                template <typename A, typename X, typename L>
                struct countable : public set<A, X> {
                    L members(A a) const {
                        static const abstractions::data::list::definition::list<L, X> requirement{};
                        return a.members();
                    }
                }; 

                template <typename A, typename X, typename L>
                struct countable<A*, X, L> : public set<A*, X> {
                    L members(A a) const {
                        static const abstractions::data::list::definition::list<L, X> requirement{};
                        return a->members();
                    }
                }; 

                template <typename A, typename X, typename L>
                struct countable<pointer<A>, X, L> : public set<pointer<A>, X> {
                    L members(A a) const {
                        static const abstractions::data::list::definition::list<L, X> requirement{};
                        return a->members();
                    }
                }; 

            }

            template <typename A>
            inline bool empty(A a) {
                return definition::existence<A>{}.empty(a);
            }

            template <typename A, typename X> 
            inline bool contains(A a, X x) {
                return definition::set<A, X>{}.contains(a, x);
            }

            template <typename A, typename X> 
            inline A remove(A a, X x) {
                return definition::removable<A, X>{}.remove(a, x);
            }

            template <typename A, typename X> 
            inline A add(A a, X x) {
                return definition::insertable<A, X>{}.add(a, x);
            }

            template <typename A, typename X, typename L> 
            inline L members(A a) {
                return definition::countable<A, X, L>{}.members(a);
            }
            
        }
    
    }
    
}

#endif
