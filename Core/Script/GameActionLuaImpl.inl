template <typename R>
R GameActionLua::RunMO(Player* player, UInt32 actionID)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "MO_%08d", actionID);
	return Run<R>(player, buffer);
}


template <typename R>
R GameActionLua::Run(Player* player, const std::string& script)
{
	Player* savePlayer1 = _player1;
	_player1 = player;

	R ret = lua_tinker::call<R>(_L, script.c_str());
	

	_player1 = savePlayer1;

	return ret;
}

template <typename R, typename T1>
inline R GameActionLua::Run(Player* player, const std::string& script, const T1& t1)
{
	Player* savePlayer1 = _player1;
	_player1 = player;

	R ret = lua_tinker::call<R, T1>(_L, script.c_str(), t1);

	_player1 = savePlayer1;

	return ret;
}

template <typename R, typename T1, typename T2>
inline R GameActionLua::Run(Player* player, const std::string& script, const T1& t1,  const T2& t2)
{
	Player* savePlayer1 = _player1;
	_player1 = player;

	R ret = lua_tinker::call<R, T1, T2>(_L, script.c_str(), t1, t2);

	_player1 = savePlayer1;

	return ret;
}

template <typename R, typename T1, typename T2, typename T3>
inline R GameActionLua::Run(Player* player, const std::string& script, const T1& t1,  const T2& t2, const T3& t3)
{
	Player* savePlayer1 = _player1;
	_player1 = player;

	R ret = lua_tinker::call<R, T1, T2, T3>(_L, script.c_str(), t1, t2, t3);

	_player1 = savePlayer1;

	return ret;
}

template <typename R, typename T1, typename T2, typename T3, typename T4>
inline R GameActionLua::Run(Player* player, const std::string& script, const T1& t1,  const T2& t2, const T3& t3, const T4& t4)
{
	Player* savePlayer1 = _player1;
	_player1 = player;

	R ret = lua_tinker::call<R, T1, T2, T3, T4>(_L, script.c_str(), t1, t2, t3, t4);

	_player1 = savePlayer1;

	return ret;
}

template<typename R>
inline R GameActionLua::Call(const std::string& name)
{
	return lua_tinker::call<R>(_L, name.c_str());
}

template<typename R, typename T1>
inline R GameActionLua::Call(const std::string& name, const T1& t1)
{
	return lua_tinker::call<R>(_L, name.c_str(), t1);
}

template<typename R, typename T1, typename T2>
inline R GameActionLua::Call(const std::string& name, const T1& t1, const T2& t2)
{
	return lua_tinker::call<R>(_L, name.c_str(), t1, t2);
}

template<typename R, typename T1, typename T2, typename T3>
inline R GameActionLua::Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3)
{
	return lua_tinker::call<R>(_L, name.c_str(), t1, t2, t3);
}

template<typename R, typename T1, typename T2, typename T3, typename T4>
inline R GameActionLua::Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3, const T4& t4)
{
	return lua_tinker::call<R>(_L, name.c_str(), t1, t2, t3, t4);
}

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
inline R GameActionLua::Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
{
	return lua_tinker::call<R>(_L, name.c_str(), t1, t2, t3, t4, t5);
}
