// -*- C++ -*- (c) 2015 Jiří Weiser

#include <tuple>
#include <memory>
#include <algorithm>
#include <utility>
#include <array>

#include "fs-utils.h"
#include "fs-inode.h"
#include "fs-constants.h"
#include "fs-file.h"
#include "fs-directory.h"
#include "fs-snapshot.h"
#include "fs-descriptor.h"
#include "fs-path.h"

#ifndef _FS_MANAGER_H_
#define _FS_MANAGER_H_

namespace divine {
namespace fs {

struct Manager {

    Manager() :
        Manager( true )
    {
        _standardIO[ 0 ]->assign( new( memory::nofail ) StandardInput() );
    }

    Manager( const char *in, size_t length ) :
        Manager( true )
    {
        _standardIO[ 0 ]->assign( new( memory::nofail ) StandardInput( in, length ) );
    }

    explicit Manager( const utils::Vector< SnapshotFS > &items ) :
        Manager()
    {
        for ( const auto &item : items )
            _insertSnapshotItem( item );
    }

    Manager( const char *in, size_t length, const utils::Vector< SnapshotFS > &items ) :
        Manager( in, length )
    {
        for ( const auto &item : items )
            _insertSnapshotItem( item );
    }

    Node findDirectoryItem( utils::String name, bool followSymLinks = true );

    void createHardLinkAt( int newdirfd, utils::String name, int olddirfd, const utils::String &target, Flags< flags::At > fl );
    void createSymLinkAt( int dirfd, utils::String name, utils::String target );
    template< typename... Args >
    Node createNodeAt( int dirfd, utils::String name, mode_t mode, Args &&... args );

    ssize_t readLinkAt( int dirfd, utils::String name, char *buf, size_t count );

    void accessAt( int dirfd, utils::String name, Flags< flags::Access > mode, Flags< flags::At > fl );
    int openFileAt( int dirfd, utils::String name, Flags< flags::Open > fl, mode_t mode );
    void closeFile( int fd );
    int duplicate( int oldfd, int lowEdge = 0 );
    int duplicate2( int oldfd, int newfd );
    std::shared_ptr< FileDescriptor > &getFile( int fd );
    std::shared_ptr< SocketDescriptor > getSocket( int sockfd );

    std::pair< int, int > pipe();

    void removeFile( utils::String name );
    void removeDirectory( utils::String name );
    void removeAt( int dirfd, utils::String name, flags::At fl );

    void renameAt( int newdirfd, utils::String newpath, int olddirfd, utils::String oldpath );

    void truncate( Node inode, off_t length );

    off_t lseek( int fd, off_t offset, Seek whence );

    template< typename DirPre, typename DirPost, typename File >
    void traverseDirectoryTree( const utils::String &root, DirPre pre, DirPost post, File file ) {
        Node current = findDirectoryItem( root );
        if ( !current || !current->mode().isDirectory() )
            return;
        if ( pre( root ) ) {
            for ( auto &i : *current->data()->as< Directory >() ) {

                if ( i.name() == "." || i.name() == ".." )
                    continue;

                utils::String pathname = path::joinPath( root, i.name() );
                if ( i.inode()->mode().isDirectory() )
                    traverseDirectoryTree( pathname, pre, post, file );
                else
                    file( pathname );
            }

            post( root );
        }
    }

    Node currentDirectory() {
        return _currentDirectory.lock();
    }

    void changeDirectory( utils::String pathname );
    void changeDirectory( int dirfd );

    void *mmap(int fd, off_t length, off_t offset, Flags<flags::Mapping> flags);
    void munmap(void *directory);

    void chmodAt( int dirfd, utils::String name, mode_t mode, Flags< flags::At > fl );
    void chmod( int fd, mode_t mode );

    mode_t umask() const {
        return _umask;
    }
    void umask( mode_t mask ) {
        _umask = Mode::GRANTS & mask;
    }

    DirectoryDescriptor *openDirectory( int fd );
    DirectoryDescriptor *getDirectory( void *descriptor );
    void closeDirectory( void *descriptor );

    int socket( SocketType type, Flags< flags::Open > fl );
    std::pair< int, int > socketpair( SocketType type, Flags< flags::Open > fl );
    void bind( int sockfd, Socket::Address address );
    void connect( int sockfd, const Socket::Address &address );
    int accept( int sockfd, Socket::Address &address );
    Node resolveAddress( const Socket::Address &address );

private:
    Node _root;
    WeakNode _currentDirectory;
    std::array< Node, 2 > _standardIO;
    utils::Vector< std::shared_ptr< FileDescriptor > > _openFD;
    utils::List< DirectoryDescriptor > _openDD;
    utils::Vector < std::unique_ptr< Memory > > _mappedMemory;

    unsigned short _umask;

    Manager( bool );// private default ctor

    std::pair< Node, utils::String > _findDirectoryOfFile( utils::String name );

    template< typename I >
    Node _findDirectoryItem( utils::String name, bool followSymLinks, I itemChecker );

    int _getFileDescriptor( std::shared_ptr< FileDescriptor > f, int lowEdge = 0 );
    void _insertSnapshotItem( const SnapshotFS &item );

    void _checkGrants( Node inode, mode_t grant ) const;

    void _chmod( Node inode, mode_t mode );



};

struct VFS {

    VFS() {
        FS_ATOMIC_SECTION_BEGIN();
        _in = nullptr;
        _length = 0;
    }
    VFS( const char *in, size_t length ) {
        FS_ATOMIC_SECTION_BEGIN();
        _in = in;
        _length = length;
    }
    explicit VFS( std::initializer_list< SnapshotFS > items ) {
        FS_ATOMIC_SECTION_BEGIN();
        _in = nullptr;
        _length = 0;
        _items.insert( _items.begin(), items );
    }
    VFS( const char *in, size_t length, std::initializer_list< SnapshotFS > items ) {
        FS_ATOMIC_SECTION_BEGIN();
        _in = in;
        _length = length;
        _items.insert( _items.begin(), items );
    }
    ~VFS() {
        FS_ATOMIC_SECTION_BEGIN();
        _manager.reset( nullptr );
    }

    // under divine mask
    Manager &instance() {
        if ( !_manager )
            _allocateManager();
        return *_manager.get();
    }

private:

    void _allocateManager() {
        if ( _items.empty() ) {
            if ( _in )
                _manager.reset( new( memory::nofail ) Manager{ _in, _length } );
            else
                _manager.reset( new( memory::nofail ) Manager{} );
        }
        else {
            if ( _in )
                _manager.reset( new( memory::nofail ) Manager{ _in, _length, _items } );
            else
                _manager.reset( new( memory::nofail ) Manager{ _items } );
            _items.clear();
        }
    }

    const char *_in;
    size_t _length;
    utils::Vector< SnapshotFS > _items;
    std::unique_ptr< Manager > _manager;
};

extern VFS vfs;

} // namespace fs
} // namespace divine

#endif
