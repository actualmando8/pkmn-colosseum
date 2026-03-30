    nofralloc
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xf
    beq @L_80222124
    cmplwi r0, 0x27
    bne @L_8022212C
@L_80222124:
    li r5, 0x1
    b @L_80222140
@L_8022212C:
    cmplwi r0, 0x16
    beq @L_8022213C
    cmplwi r0, 0x2e
    bne @L_80222140
@L_8022213C:
    li r5, 0x0
@L_80222140:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80222154
    cmplwi r0, 0x4
    bne @L_8022216C
@L_80222154:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222168
    li r3, 0x26
    b @L_8022216C
@L_80222168:
    li r3, 0x27
@L_8022216C:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_80222188
    cmplwi r0, 0x5
    beq @L_80222188
    cmplwi r0, 0x7
    bne @L_802221A0
@L_80222188:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022219C
    li r3, 0x28
    b @L_802221A0
@L_8022219C:
    li r3, 0x29
@L_802221A0:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x3
    bne @L_802221C4
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802221C0
    li r3, 0x2a
    b @L_802221C4
@L_802221C0:
    li r3, 0x2b
@L_802221C4:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x6
    bnelr
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802221E4
    li r3, 0x2c
    blr
@L_802221E4:
    li r3, 0x3c
    blr
