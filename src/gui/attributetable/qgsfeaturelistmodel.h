#ifndef QGSATTRIBUTEEDITORMODEL_H
#define QGSATTRIBUTEEDITORMODEL_H

#include <qgsexpression.h>

#include <QAbstractProxyModel>
#include <QVariant>
#include <QItemSelectionModel>

#include "qgsfeaturemodel.h"
#include "qgsfeature.h" // QgsFeatureId

class QgsAttributeTableFilterModel;
class QgsAttributeTableModel;
class QgsVectorLayerCache;

class GUI_EXPORT QgsFeatureListModel : public QAbstractProxyModel, public QgsFeatureModel
{
    Q_OBJECT

  public:
    struct FeatureInfo
    {
    public:
      FeatureInfo()
          : isNew( false )
          , isEdited( false )
      {}

      bool isNew;
      bool isEdited;
    };

  public:
    explicit QgsFeatureListModel( QgsAttributeTableFilterModel *sourceModel, QObject* parent = NULL );
    virtual ~QgsFeatureListModel();

    virtual void setSourceModel( QgsAttributeTableFilterModel* sourceModel );
    QgsVectorLayerCache* layerCache();
    virtual QVariant data( const QModelIndex& index, int role ) const;
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;

    QgsAttributeTableModel* masterModel();

    /**
     *  @param  expression   A {@link QgsExpression} compatible string.
     *  @return true if the expression could be set, false if there was a parse error.
     *          If it fails, the old expression will still be applied. Call {@link parserErrorString()}
     *          for a meaningful error message.
     */
    bool setDisplayExpression( const QString expression );

    /**
     * @brief Returns a detailed message about errors while parsing a QgsExpression.
     * @return A message containg information about the parser error.
     */
    QString parserErrorString();

    const QString displayExpression() const;
    bool featureByIndex( const QModelIndex& index, QgsFeature& feat );
    QgsFeatureId idxToFid( const QModelIndex& index ) const;
    QModelIndex fidToIdx( const QgsFeatureId fid ) const;

    virtual QModelIndex mapToSource( const QModelIndex& proxyIndex ) const;
    virtual QModelIndex mapFromSource( const QModelIndex& sourceIndex ) const;

    virtual QModelIndex mapToMaster( const QModelIndex& proxyIndex ) const;
    virtual QModelIndex mapFromMaster( const QModelIndex& sourceIndex ) const;

    virtual QItemSelection mapSelectionFromMaster( const QItemSelection& selection ) const;
    virtual QItemSelection mapSelectionToMaster( const QItemSelection& selection ) const;

    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& child ) const;
    virtual int columnCount( const QModelIndex&parent = QModelIndex() ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;

    QModelIndex fidToIndex( QgsFeatureId fid );
    QModelIndexList fidToIndexList( QgsFeatureId fid );

  public slots:
    void onBeginRemoveRows( const QModelIndex& parent, int first, int last );
    void onEndRemoveRows( const QModelIndex& parent, int first, int last );
    void onBeginInsertRows( const QModelIndex& parent, int first, int last );
    void onEndInsertRows( const QModelIndex& parent, int first, int last );


  private:
    QgsExpression* mExpression;
    QgsAttributeTableFilterModel* mFilterModel;
    QString mParserErrorString;
};

Q_DECLARE_METATYPE( QgsFeatureListModel::FeatureInfo )

#endif // QGSATTRIBUTEEDITORMODEL_H
