//
//  CouchModel.h
//  CouchCocoa
//
//  Created by Jens Alfke on 8/26/11.
//  Copyright (c) 2011 Couchbase, Inc. All rights reserved.
//

#import "CouchDynamicObject.h"
@class CouchDatabase, CouchDocument, RESTOperation;


/** Generic model class for Couch documents.
    You can subclass this and declare properties in the subclass's @interface without needing to implement their accessor methods or declare instance variables; simply note them as '@dynamic' in the @implementation.
    The property value will automatically be fetched from or stored to the document, using the same name. */
@interface CouchModel : CouchDynamicObject
{
    @private
    CouchDocument* _document;
    CFAbsoluteTime _changedTime;
    bool _autosaves :1;
    bool _isNew     :1;
    bool _needsSave :1;

    NSMutableDictionary* _properties;
    NSMutableSet* _changedNames;
}

/** Returns the CouchModel associated with a CouchDocument, or creates & assigns one if necessary.
    Don't call this on CouchModel itself, rather on the subclass you want to instantiate for that document, e.g. [MyWidgetModel modelForDocument: doc]. It always returns an instance of the class it's called on. */
+ (id) modelForDocument: (CouchDocument*)document;

/** Creates a new "untitled" model object with no document yet.
    Setting its -database property will cause it to create and save a CouchDocument. */
- (id) init;

/** The document this item is associated with. Will be nil if it's new and unsaved. */
@property (readonly, retain) CouchDocument* document;

/** The database the item's document belongs to.
    Setting this property will assign the item to a database, creating a document.
    Setting it to nil will delete its document from its database. */
@property (retain) CouchDatabase* database;

/** Is this model new, never before saved? */
@property (readonly) bool isNew;


/** Writes any changes to a new revision of the document, asynchronously.
    Does nothing and returns nil if no changes have been made. */
- (RESTOperation*) save;

/** Should changes be saved back to the database automatically?
    Defaults to NO, requiring you to call -save manually. */
@property (nonatomic) bool autosaves;

/** Does this model have unsaved changes? */
@property (readonly) bool needsSave;

/** The document's current properties, in externalized JSON format. */
- (NSDictionary*) propertiesToSave;


- (void) deleteDocument;


/** The time interval since the document was last changed externally (e.g. by a "pull" replication.
    This value can be used to highlight recently-changed objects in the UI. */
@property (readonly) NSTimeInterval timeSinceExternallyChanged;

- (void) markExternallyChanged;


// PROTECTED (SUBCLASSES ONLY):

/** Designated initializer. Do not call directly except from subclass initializers; to create a new instance call +modelForDocument: instead.
    @param document  The document. Nil if this is created new (-init was called). */
- (id) initWithDocument: (CouchDocument*)document;

/** The document ID to use when creating a new document.
    Default is nil, which means to assign no ID (the server will assign one). */
- (NSString*) idForNewDocument;

/** Called when the model's properties are reloaded from the document.
    This happens both when initialized from a document, and after an external change. */
- (void) didLoadFromDocument;

@end
