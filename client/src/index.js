import React from 'react';
import ReactDOM from 'react-dom';
import { BrowserRouter as Router, Route, Link } from 'react-router-dom';
import { Provider } from 'react-redux';
import { browserHistory } from 'react-router'
import { Formik } from 'formik';
import configureStore from './store/configureStore';
import CampaignsIndex from './components/campaignsIndex';
import LanguagesIndex from './components/languagesIndex';
import ContentIndex from './components/contentIndex';
import AudienceIndex from './components/audienceIndex';

const store = configureStore();

const BasicExample = () => (
  <Router history={browserHistory} >
    <div>
      <ul>
        <li>
          <Link to='/'>Home</Link>
        </li>
        <li>
          <Link to='/campaigns'>Campaigns</Link>
        </li>
        <li>
          <Link to='/content'>Content</Link>
        </li>
        <li>
          <Link to='/audience'>Audience</Link>
        </li>
        <li>
          <Link to='/languages'>Languages</Link>
        </li>
        <li>
          <Link to='/settings'>Settings</Link>
        </li>
      </ul>
      <hr />
      <Route exact path='/' component={Home} />
      <Route exact path='/campaigns' component={Campaigns} />
      <Route exact path='/content' component={Content} />
      <Route exact path='/audience' component={Audience} />
      <Route exact path='/languages' component={Languages} />
      <Route exact path='/languages/create' component={CreateLanguage} />
      <Route exact path='/settings' component={Settings} />
    </div>
  </Router>
);

const Home = () => (
  <div>
    <h2>Home</h2>
  </div>
);

const Campaigns = () => (
  <div>
    <h2>Campaigns</h2>
    <CampaignsIndex />
  </div>
);

const Content = () => (
  <div>
    <h2>Content</h2>
    <ContentIndex />
  </div>
);

const Audience = () => (
  <div>
    <h2>Audience</h2>
    <AudienceIndex />
  </div>
);

const Languages = () => (
  <div>
    <h2>Languages</h2>
    <LanguagesIndex />
  </div>
);

const CreateLanguage = () => (
  <div>
    <h2>Add language</h2>
    <Formik
      initialValues={{
        name: ''
      }}
      validate={values => {
        let errors = {};
        if (!values.name) {
          errors.name = 'This field is required.';
        } 
        return errors;
      }}
      onSubmit={(values) => {
        store.dispatch();
      }}
      render={({
        values,
        errors,
        touched,
        handleChange,
        handleBlur,
        handleSubmit,
        isSubmitting
      }) => (
        <form onSubmit={handleSubmit}>
          <input 
            type='text' 
            name='name'
            onChange={handleChange}
            onBlur={handleBlur}
            value={values.name} 
          />
          {touched.name && errors.name && <div>{errors.name}</div>}
          <button type='submit' disabled={isSubmitting}>
            Submit
          </button>
        </form>
      )}
    />
  </div>
);

const Settings = () => (
  <div>
    <h2>Settings</h2>
  </div>
);

ReactDOM.render(
  <Provider store={store}>
    <BasicExample />
  </Provider>, 
  document.getElementById('app')
);
