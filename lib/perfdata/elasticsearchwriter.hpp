/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#ifndef ELASTICSEARCHWRITER_H
#define ELASTICSEARCHWRITER_H

#include "perfdata/elasticsearchwriter-ti.hpp"
#include "icinga/service.hpp"
#include "base/configobject.hpp"
#include "base/workqueue.hpp"
#include "base/timer.hpp"
#include "base/tlsstream.hpp"

namespace icinga
{

class ElasticsearchWriter final : public ObjectImpl<ElasticsearchWriter>
{
public:
	DECLARE_OBJECT(ElasticsearchWriter);
	DECLARE_OBJECTNAME(ElasticsearchWriter);

	static void StatsFunc(const Dictionary::Ptr& status, const Array::Ptr& perfdata);

	static String FormatTimestamp(double ts);

	void ValidateHostTagsTemplate(const Lazy<Dictionary::Ptr> &lvalue, const ValidationUtils &utils) override;
	void ValidateServiceTagsTemplate(const Lazy<Dictionary::Ptr> &lvalue, const ValidationUtils &utils) override;

protected:
	void OnConfigLoaded() override;
	void Resume() override;
	void Pause() override;

private:
	String m_EventPrefix;
	WorkQueue m_WorkQueue{10000000, 1};
	boost::signals2::connection m_HandleCheckResults, m_HandleStateChanges, m_HandleNotifications;
	Timer::Ptr m_FlushTimer;
	std::vector<String> m_DataBuffer;
	std::mutex m_DataBufferMutex;

	void AddCheckResult(const Dictionary::Ptr& fields, const Checkable::Ptr& checkable, const CheckResult::Ptr& cr);
	void AddTemplateTags(const Dictionary::Ptr& fields, const Checkable::Ptr& checkable, const CheckResult::Ptr& cr);

	void StateChangeHandler(const Checkable::Ptr& checkable, const CheckResult::Ptr& cr);
	void CheckResultHandler(const Checkable::Ptr& checkable, const CheckResult::Ptr& cr);
	void NotificationSentToAllUsersHandler(const Checkable::Ptr& checkable, const std::set<User::Ptr>& users,
		NotificationType type, const CheckResult::Ptr& cr, const String& author, const String& text);

	void Enqueue(const Checkable::Ptr& checkable, const String& type,
		const Dictionary::Ptr& fields, double ts);

	OptionalTlsStream Connect();
	void AssertOnWorkQueue();
	void ExceptionHandler(boost::exception_ptr exp);
	void FlushTimeout();
	void Flush();
	void SendRequest(const String& body);
};

}

#endif /* ELASTICSEARCHWRITER_H */
