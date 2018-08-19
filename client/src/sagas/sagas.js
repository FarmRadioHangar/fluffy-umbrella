import { fork, all, call, put, takeEvery } from 'redux-saga/effects';
import * as types from '../actions/actionTypes';  
import * as languagesActions from '../actions/languagesActions';
import api from '../api';

function* callGetLanguagesSaga(action) {
  try {
    const { offset, limit } = action;
    const response = yield call(api.get, 'languages', { skip: offset, limit });
    if (response.ok) {
      yield put(languagesActions.fetchLanguagesDone(response));
    } else {
      yield put(languagesActions.fetchLanguagesFailed(`API error: ${response.error}`));
    }
  } catch(err) {
    yield put(languagesActions.fetchLanguagesFailed('Could not connect to API server.'));
  }
}

function* getLanguagesSaga() {
  yield takeEvery(types.FETCH_LANGUAGES, callGetLanguagesSaga);
}

export function* rootSaga() {
  yield all([
    fork(getLanguagesSaga)
  ]);
}
