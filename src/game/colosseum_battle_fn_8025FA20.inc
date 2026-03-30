    nofralloc
    clrlwi r0, r3, 16
    lfs f1, -20496(r2)
    cmpwi r0, 0x92
    beq @L_8025FB38
    bge @L_8025FAA4
    cmpwi r0, 0x4a
    beq @L_8025FB98
    bge @L_8025FA70
    cmpwi r0, 0x26
    beq @L_8025FB10
    bge @L_8025FA64
    cmpwi r0, 0x1a
    beq @L_8025FB60
    bge @L_8025FBAC
    cmpwi r0, 0x6
    beq @L_8025FB68
    b @L_8025FBAC
@L_8025FA64:
    cmpwi r0, 0x44
    beq @L_8025FB88
    b @L_8025FBAC
@L_8025FA70:
    cmpwi r0, 0x8e
    beq @L_8025FB30
    bge @L_8025FA94
    cmpwi r0, 0x85
    beq @L_8025FBA8
    bge @L_8025FBAC
    cmpwi r0, 0x4c
    bge @L_8025FBAC
    b @L_8025FB18
@L_8025FA94:
    cmpwi r0, 0x90
    beq @L_8025FB40
    bge @L_8025FB20
    b @L_8025FBAC
@L_8025FAA4:
    cmpwi r0, 0x136
    beq @L_8025FB90
    bge @L_8025FAE0
    cmpwi r0, 0xfa
    beq @L_8025FB48
    bge @L_8025FAD4
    cmpwi r0, 0xe2
    beq @L_8025FB70
    bge @L_8025FBAC
    cmpwi r0, 0xd9
    beq @L_8025FB78
    b @L_8025FBAC
@L_8025FAD4:
    cmpwi r0, 0x12c
    beq @L_8025FB28
    b @L_8025FBAC
@L_8025FAE0:
    cmpwi r0, 0x16b
    beq @L_8025FBA0
    bge @L_8025FAF8
    cmpwi r0, 0x14b
    beq @L_8025FB80
    b @L_8025FBAC
@L_8025FAF8:
    cmpwi r0, 0x198
    beq @L_8025FB58
    bge @L_8025FBAC
    cmpwi r0, 0x197
    bge @L_8025FB50
    b @L_8025FBAC
@L_8025FB10:
    lfs f1, -20492(r2)
    b @L_8025FBAC
@L_8025FB18:
    lfs f1, -20488(r2)
    b @L_8025FBAC
@L_8025FB20:
    lfs f1, -20484(r2)
    b @L_8025FBAC
@L_8025FB28:
    lfs f1, -20480(r2)
    b @L_8025FBAC
@L_8025FB30:
    lfs f1, -20484(r2)
    b @L_8025FBAC
@L_8025FB38:
    lfs f1, -20476(r2)
    b @L_8025FBAC
@L_8025FB40:
    lfs f1, -20476(r2)
    b @L_8025FBAC
@L_8025FB48:
    lfs f1, -20488(r2)
    b @L_8025FBAC
@L_8025FB50:
    lfs f1, -20476(r2)
    b @L_8025FBAC
@L_8025FB58:
    lfs f1, -20476(r2)
    b @L_8025FBAC
@L_8025FB60:
    lfs f1, -20480(r2)
    b @L_8025FBAC
@L_8025FB68:
    lfs f1, -20480(r2)
    b @L_8025FBAC
@L_8025FB70:
    lfs f1, -20484(r2)
    b @L_8025FBAC
@L_8025FB78:
    lfs f1, -20484(r2)
    b @L_8025FBAC
@L_8025FB80:
    lfs f1, -20472(r2)
    b @L_8025FBAC
@L_8025FB88:
    lfs f1, -20472(r2)
    b @L_8025FBAC
@L_8025FB90:
    lfs f1, -20484(r2)
    b @L_8025FBAC
@L_8025FB98:
    lfs f1, -20488(r2)
    b @L_8025FBAC
@L_8025FBA0:
    lfs f1, -20488(r2)
    b @L_8025FBAC
@L_8025FBA8:
    lfs f1, -20488(r2)
@L_8025FBAC:
    cmplwi r5, 0x0
    beq @L_8025FBBC
    lfs f0, -20468(r2)
    stfs f0, 0x0(r5)
@L_8025FBBC:
    cmplwi r4, 0x0
    beqlr
    stfs f1, 0x0(r4)
    blr
